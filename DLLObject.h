#pragma once
#include <Essentials.h>
#include <LibraTestProj/LibraTestDLL/IScriptObject.h>

namespace Libra {
	class DLLObject
	{
	public:
		const HMODULE& LoadDLL(const std::string& path);

		const HMODULE& GetModule();

		IScriptObject* GetComponent();

		~DLLObject();

		void Free();

	private:
		HMODULE hModule_;

		IScriptObject* component_;
	};
}

