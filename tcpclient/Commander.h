
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
	deque<char*> task_sequence;	//������� 

	char* get_task();			// ȡ��ǰ�����д���ɵ�����
	bool execute();			// ִ������ִ�гɹ� ɾ�������
	bool add_task();			// ��������� ����������� ʧ��



};

