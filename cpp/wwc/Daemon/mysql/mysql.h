#include <string>
#include <mysql/mysql.h>
#include <malloc.h>

using namespace std;

class PMySQL
{
        public:
                PMySQL();
                ~PMySQL();

                int connect_mysql(string host, string user, string passwd, string database, int port);

                string get_project_content_by_code(string code);
                int add_project_content_by_code(string code, string content);
                string get_task_content_by_code(string code);
                int add_task_conent_by_code(string code, string content);
                
                // 根据编码获取操作类型和操作代码
                // 编码正确且未过期，返回操作代码，pkind为操作类型，如果pkind为NULL则代表操作代码不正确或过期
                // 操作类型：0代表项目、1代表任务
                string get_action_by_number(string number, int* pkind);

                void    set_number_invail(string code);
        private:
                MYSQL m_mysql;
                bool isConnect;
                int query(string sql);
                string get_first_data();
        protected:
};
