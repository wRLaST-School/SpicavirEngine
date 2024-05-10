#include <SpFramework.h>
#include <Essentials.h>

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_  HINSTANCE, _In_ LPSTR, _In_ int)
{
	SpFramework fw;

	fw.Init();

	fw.Run();

	return 0;
}