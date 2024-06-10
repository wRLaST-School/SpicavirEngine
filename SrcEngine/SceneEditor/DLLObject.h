#pragma once
#include <Essentials.h>
#include <LibraTestProj/LibraTestDLL/IScriptObject.h>

namespace Libra {
	class DLLExport DLLObject
	{
	public:
		const HMODULE& LoadDLL(const std::string& className);

		const HMODULE& GetModule();

		IScriptObject* GetComponent();

		~DLLObject();

		void Free();

	private:
		HMODULE hModule_;

		IScriptObject* component_;
	};
}

