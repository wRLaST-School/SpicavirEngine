#include "ExprAST.h"
#include <Windows.h>
#include <string>
#include <format>
#include <map>

namespace Libra {
    static std::unique_ptr<llvm::LLVMContext> TheContext;
    static std::unique_ptr<llvm::IRBuilder<>> Builder;
    static std::unique_ptr<llvm::Module> TheModule;
    static std::map<std::string, llvm::Value*> NamedValues;

    void InitLLVM()
    {
        Builder = std::make_unique<llvm::IRBuilder<>>(*(TheContext.get()));
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
        return llvm::ConstantFP::get(*TheContext, llvm::APFloat(val_));
    }
    llvm::Value* VariableExprAST::CodeGen()
    {
        llvm::Value* v = NamedValues[name_];
        return v ? v : ErrorV("Unknown variable name");
    }
    llvm::Value* BinaryExprAST::CodeGen()
    {
        llvm::Value* l = lhs_->CodeGen();
        llvm::Value* r = rhs_->CodeGen();

        if (l == 0 || r == 0) return 0;

        switch (op_)
        {
        case '+': return Builder->CreateFAdd(l, r, "addtmp");
        case '-': return Builder->CreateFSub(l, r, "subtmp");
        case '*': return Builder->CreateFMul(l, r, "multmp");
        case '/': return Builder->CreateFDiv(l, r, "divtmp");
        case '<' :
            l = Builder->CreateFCmpULT(l, r, "cmptmp");
            return Builder->CreateUIToFP(l, llvm::Type::getFloatTy(*TheContext));
        case '>':
            l = Builder->CreateFCmpULT(r, l, "cmptmp");
            return Builder->CreateUIToFP(l, llvm::Type::getFloatTy(*TheContext));

        default:
            return ErrorV("invalid binary operator");
        }
    }
    llvm::Value* CallExprAST::CodeGen()
    {
        llvm::Function* calleeF = TheModule->getFunction(callee_);
        if (calleeF == 0) return ErrorV("Unknown function referenced");

        if (calleeF->arg_size() != args_.size())
            return ErrorV("Incorrect arguments passed (size does not match)");

        std::vector<llvm::Value*> argsV;
        for (unsigned i = 0, e = (unsigned)args_.size(); i != e; ++i)
        {
            argsV.push_back(args_[i]->CodeGen());
            if (argsV.back() == 0) return 0;
        }

        return Builder->CreateCall(calleeF, argsV, "calltmp");
    }
    llvm::Function* PrototypeAST::CodeGen()
    {
        // float(float, float)などの関数型を作る
        std::vector<llvm::Type*> floats(args_.size(),
            llvm::Type::getFloatTy(*TheContext));

        llvm::FunctionType* ft = llvm::FunctionType::get(llvm::Type::getFloatTy(*TheContext),
            floats, false);

        llvm::Function* f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name_, *TheModule);

        if (f->getName() != name_) // fのnameが元のname_と一致しないなら、再定義であるため元の物を持ってくる
        {
            f->eraseFromParent();
            f = TheModule->getFunction(name_);

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
            NamedValues[args_[idx]] = ai;
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

        llvm::BasicBlock* bb = llvm::BasicBlock::Create(*TheContext, "entry", theFunc);
        Builder->SetInsertPoint(bb);

        if (llvm::Value* retval = body_->CodeGen()) {
            Builder->CreateRet(retval);

            llvm::verifyFunction(*theFunc);

            return theFunc;
        }

        //コード生成エラーがあったらその旨を出力
        Error(std::format("error compiling func: {}", proto_->GetName()).c_str());

        return 0;
    }
}
