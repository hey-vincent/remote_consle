#pragma once
class Execute
{
public:
	Execute();
	~Execute();

	bool Run();
	bool kill_process(LPCTSTR strProcessName);

};

