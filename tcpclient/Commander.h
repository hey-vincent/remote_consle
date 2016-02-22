
#include <deque>
#include <string>
#include <map>

using namespace std;

class Commander
{
public:
	Commander(/*char* cmd, int nlen*/);
	~Commander();

private:
	int initialize();
	int load_setting();
	void parse_setting(string setting);



	map<string, string> m_map_settings;
	deque<char*> task_sequence;	//任务队列 

	char* get_task();			// 取当前队列中待完成的任务
	bool execute();			// 执行任务：执行成功 删除待完成
	bool add_task();			// 添加新任务。 任务队列排满 失败



};

