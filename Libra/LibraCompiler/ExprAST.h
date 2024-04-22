#pragma once
#include <string>
#include <vector>
#include <memory>
#include "LLVMIncludes.h"

namespace Libra {
	extern void InitLLVM();

	class ExprAST
	{
	public:
		virtual ~ExprAST() {};
		virtual llvm::Value* CodeGen() = 0;
	};

	//数値を参照する式クラス(一旦全部数値はfloatで扱う)
	class NumberExprAST : public ExprAST {
	private:
		float val_;

	public:
		NumberExprAST(float val) : val_(val) {}
		llvm::Value* CodeGen() override;
	};

	//変数を参照するための式クラス
	class VariableExprAST : public ExprAST {
	private:
		std::string name_;

	public:
		VariableExprAST(const std::string& name) : name_(name) {}
		llvm::Value* CodeGen() override;
	};

	//演算子の処理のための式クラス
	//演算子の優先順位は未実装
	class BinaryExprAST : public ExprAST {
	private:
		char op_;

		std::unique_ptr<ExprAST> lhs_, rhs_;

	public:
		BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
			: op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
		llvm::Value* CodeGen() override;
	};

	//関数呼び出しのための式クラス
	class CallExprAST : public ExprAST {
		std::string callee_;
		std::vector<std::unique_ptr<ExprAST>> args_{};
	public:
		CallExprAST(const std::string& callee, std::vector<std::unique_ptr<ExprAST>> args)
			: callee_(callee), args_(std::move(args)) {}
		llvm::Value* CodeGen() override;
	};

	//関数のプロトタイプ宣言を表すクラス
	class PrototypeAST {
		std::string name_;
		std::vector<std::string> args_;
	public:
		PrototypeAST(const std::string& name, const std::vector<std::string>& args)
			: name_(name), args_(args) {}
		llvm::Function* CodeGen();

		std::string GetName();
	};

	//関数定義を表すクラス
	class FunctionAST {
		std::unique_ptr<PrototypeAST> proto_;
		std::unique_ptr<ExprAST> body_;
	public:
		FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body)
			: proto_(std::move(proto)), body_(std::move(body)) {}
		llvm::Function* CodeGen() ;
	};

	// If文による制御を表すクラス
	class IfExprAST : public ExprAST {
		std::unique_ptr<ExprAST> cond_, then_, else_;
	public:
		IfExprAST(std::unique_ptr<ExprAST> cond, 
			std::unique_ptr<ExprAST> then, 
			std::unique_ptr<ExprAST> arg_else)
			: cond_(std::move(cond)), then_(std::move(then)), else_(std::move(arg_else)) {}

		virtual llvm::Value* CodeGen();
	};

	llvm::Value* ErrorV(const char* str);
	std::unique_ptr<ExprAST> Error(const char* str);
	std::unique_ptr<PrototypeAST> ErrorP(const char* str);
	std::unique_ptr<FunctionAST> ErrorF(const char* str);
}

