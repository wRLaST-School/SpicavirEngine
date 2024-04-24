#include "ExprAST.h"
#include <Windows.h>
#include <string>
#include <format>
#include <map>
#include "CodeObject.h"
#define nullret(v) if(!v) return nullptr


namespace Libra {
    void InitLLVM()
    {
        // Open a new context and module.
        CodeObject::TheContext = std::make_unique<llvm::LLVMContext>();
        CodeObject::TheModule = std::make_unique<llvm::Module>("Testing JIT", *CodeObject::TheContext);

        CodeObject::Builder = std::make_unique<llvm::IRBuilder<>>(*(CodeObject::TheContext.get()));
    }

    llvm::Value* ErrorV(const char* str) {
        Error(str);
        return nullptr;
    }

    std::unique_ptr<ExprAST> Libra::Error(const char* str)
    {
        OutputDebugStringA(std::format("Error: {}\n", str).c_str());
        return nullptr;
    }

    std::unique_ptr<PrototypeAST> Libra::ErrorP(const char* str)
    {
        Error(str);
        return nullptr;
    }

    std::unique_ptr<FunctionAST> Libra::ErrorF(const char* str)
    {
        Error(str);
        return nullptr;
    }

    llvm::Value* NumberExprAST::CodeGen()
    {
        return llvm::ConstantFP::get(*CodeObject::TheContext, llvm::APFloat(val_));
    }
    llvm::Value* VariableExprAST::CodeGen()
    {
        llvm::Value* v = CodeObject::NamedValues[name_];
        return v ? v : ErrorV("Unknown variable name");
    }
    llvm::Value* BinaryExprAST::CodeGen()
    {
        llvm::Value* l = lhs_->CodeGen();
        llvm::Value* r = rhs_->CodeGen();

        if (l == 0 || r == 0) return 0;

        switch (op_)
        {
        case '+': return CodeObject::Builder->CreateFAdd(l, r, "addtmp");
        case '-': return CodeObject::Builder->CreateFSub(l, r, "subtmp");
        case '*': return CodeObject::Builder->CreateFMul(l, r, "multmp");
        case '/': return CodeObject::Builder->CreateFDiv(l, r, "divtmp");
        case '<' :
            l = CodeObject::Builder->CreateFCmpULT(l, r, "cmptmp");
            return CodeObject::Builder->CreateUIToFP(l, llvm::Type::getFloatTy(*CodeObject::TheContext));
        case '>':
            l = CodeObject::Builder->CreateFCmpULT(r, l, "cmptmp");
            return CodeObject::Builder->CreateUIToFP(l, llvm::Type::getFloatTy(*CodeObject::TheContext));

        default:
            return ErrorV("invalid binary operator");
        }
    }
    llvm::Value* CallExprAST::CodeGen()
    {
        llvm::Function* calleeF = CodeObject::TheModule->getFunction(callee_);
        if (calleeF == 0) return ErrorV("Unknown function referenced");

        if (calleeF->arg_size() != args_.size())
            return ErrorV("Incorrect arguments passed (size does not match)");

        std::vector<llvm::Value*> argsV;
        for (unsigned i = 0, e = (unsigned)args_.size(); i != e; ++i)
        {
            argsV.push_back(args_[i]->CodeGen());
            if (argsV.back() == 0) return 0;
        }

        return CodeObject::Builder->CreateCall(calleeF, argsV, "calltmp");
    }
    llvm::Function* PrototypeAST::CodeGen()
    {
        // float(float, float)などの関数型を作る
        std::vector<llvm::Type*> floats(args_.size(),
            llvm::Type::getFloatTy(*CodeObject::TheContext));

        llvm::FunctionType* ft = llvm::FunctionType::get(llvm::Type::getFloatTy(*CodeObject::TheContext),
            floats, false);

        llvm::Function* f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name_, *CodeObject::TheModule);

        if (f->getName() != name_) // fのnameが元のname_と一致しないなら、再定義であるため元の物を持ってくる
        {
            f->eraseFromParent();
            f = CodeObject::TheModule->getFunction(name_);

            //すでにfが本体を持つなら受け入れない
            if (!f->empty()) {
                ErrorF("redefinition of function");
                return 0;
            }

            //もしfが受け取る引数の数が違うなら受け入れない
            if (f->arg_size() != args_.size()) {
                ErrorF("redefinition of function with different args");
                return 0;
            }
        }

        //チェックを通ったら全ての引数に名前をつける
        unsigned idx = 0;
        for (llvm::Function::arg_iterator ai = f->arg_begin(); idx != args_.size();
            ++ai, ++idx) {
            ai->setName(args_[idx]);

            // 変数シンボルテーブルに引数を追加する。
            CodeObject::NamedValues[args_[idx]] = reinterpret_cast<llvm::AllocaInst*>(ai);
        }
        return f;
    }
    std::string PrototypeAST::GetName()
    {
        return name_;
    }
    llvm::Function* FunctionAST::CodeGen()
    {
        llvm::Function* theFunc = proto_->CodeGen();

        if (theFunc == 0) return 0;

        llvm::BasicBlock* bb = llvm::BasicBlock::Create(*CodeObject::TheContext, "entry", theFunc);
        CodeObject::Builder->SetInsertPoint(bb);

        if (llvm::Value* retval = body_->CodeGen()) {
            CodeObject::Builder->CreateRet(retval);

            llvm::verifyFunction(*theFunc);

            return theFunc;
        }

        //コード生成エラーがあったらその旨を出力
        Error(std::format("error compiling func: {}", proto_->GetName()).c_str());

        return 0;
    }
    llvm::Value* IfExprAST::CodeGen()
    {
        llvm::Value* condV = cond_->CodeGen();
        nullret(condV);

        condV = CodeObject::Builder->CreateFCmpONE(condV,
            llvm::ConstantFP::get(*CodeObject::TheContext, llvm::APFloat(0.0)), "ifcond");

        llvm::Function* theFunc = CodeObject::Builder->GetInsertBlock()->getParent();

        //thenとelseのブロックを生成
        llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(*CodeObject::TheContext, "then", theFunc);
        llvm::BasicBlock* elseBB = llvm::BasicBlock::Create(*CodeObject::TheContext, "else");
        llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(*CodeObject::TheContext, "ifcont");

        CodeObject::Builder->CreateCondBr(condV, thenBB, elseBB);

        CodeObject::Builder->SetInsertPoint(thenBB);

        llvm::Value* thenV = then_->CodeGen();
        nullret(thenV);

        CodeObject::Builder->CreateBr(mergeBB);

        thenBB = CodeObject::Builder->GetInsertBlock();

        // elseブロックを発行する
        theFunc->insert(theFunc->end(), elseBB);

        CodeObject::Builder->SetInsertPoint(elseBB);

        llvm::Value* elseV = else_->CodeGen();

        nullret(elseV);

        CodeObject::Builder->CreateBr(mergeBB);

        elseBB = CodeObject::Builder->GetInsertBlock();

        // mergeブロックを発行する
        theFunc->insert(theFunc->end(), mergeBB);

        CodeObject::Builder->SetInsertPoint(mergeBB);

        llvm::PHINode* PN = CodeObject::Builder->CreatePHI(
            llvm::Type::getFloatTy(*CodeObject::TheContext),
            2, "iftmp"
        );

        PN->addIncoming(thenV, thenBB);
        PN->addIncoming(elseV, elseBB);

        return PN;
    }
    llvm::Value* ForExprAST::CodeGen()
    {
        //スコープ内の変数なしで開始コードを出力する
        llvm::Value* startVal = start_->CodeGen();
        nullret(startVal);

        // ループのヘッダ用の基本ブロックを作成
        llvm::Function* theFunc = CodeObject::Builder->GetInsertBlock()->getParent();
        llvm::BasicBlock* preHeaderBB = CodeObject::Builder->GetInsertBlock();
        llvm::BasicBlock* loopBB = llvm::BasicBlock::Create(*CodeObject::TheContext, "loop", theFunc);

        CodeObject::Builder->CreateBr(loopBB);

        CodeObject::Builder->SetInsertPoint(loopBB);

        llvm::PHINode* variable = CodeObject::Builder->CreatePHI(
            llvm::Type::getFloatTy(*CodeObject::TheContext), 2, varName_);
        variable->addIncoming(startVal, preHeaderBB);

        llvm::Value* oldVal = CodeObject::NamedValues[varName_];
        CodeObject::NamedValues[varName_] = reinterpret_cast<llvm::AllocaInst*>(variable);

        if (body_->CodeGen() == nullptr)
            return 0;

        llvm::Value* stepVal;

        if (step_) {
            stepVal = step_->CodeGen();
            nullret(stepVal);
        }
        else {
            stepVal = llvm::ConstantFP::get(*CodeObject::TheContext, llvm::APFloat(1.0));
        }

        llvm::Value* nextVar = CodeObject::Builder->CreateFAdd(variable, stepVal, "nextvar");

        // 終了条件
        llvm::Value* endCond = end_->CodeGen();
        nullret(endCond);

        endCond = CodeObject::Builder->CreateFCmpONE(endCond,
            llvm::ConstantFP::get(*CodeObject::TheContext, llvm::APFloat(0.0)), "loopcond");

        llvm::BasicBlock* loopEndBB = CodeObject::Builder->GetInsertBlock();
        llvm::BasicBlock* afterBB = llvm::BasicBlock::Create(*CodeObject::TheContext, "afterloop", theFunc);

        CodeObject::Builder->CreateCondBr(endCond, loopBB, afterBB);

        CodeObject::Builder->SetInsertPoint(afterBB);

        variable->addIncoming(nextVar, loopEndBB);

        //スコープ外に同名変数があった場合復元する
        if (oldVal)
            CodeObject::NamedValues[varName_] = reinterpret_cast<llvm::AllocaInst*>(oldVal);
        else
            CodeObject::NamedValues.erase(varName_);

        // forは常に0.0fを返す
        return llvm::Constant::getNullValue(llvm::Type::getFloatTy(*CodeObject::TheContext));
    }
}
