#include "mysql.h"
#include <stdlib.h>

string&   replace_all_distinct(string&   str,const   string&   old_value,const   string&   new_value)     
{     
        for(string::size_type   pos(0);   pos!=string::npos;   pos+=new_value.length()){     
                if((pos=str.find(old_value,pos))!=string::npos)
                        str.replace(pos,old_value.length(),new_value);     
                else   
                        break;     
        }     
        return   str;     
}    

PMySQL::PMySQL()
{
        mysql_init(&m_mysql);
        isConnect       = false;
}

PMySQL::~PMySQL()
{
        isConnect       = false;
        mysql_close(&m_mysql);
}

// error return -1,otherwise return 0
int PMySQL::connect_mysql(string host, string user, string passwd, string database, int port)
{
        if(!mysql_real_connect(&m_mysql, (const char*)host.c_str(), (const char*)user.c_str(), (const char*)passwd.c_str(), (const char*)database.c_str(), port, NULL, 0)){
                return -1;
        }
        isConnect       = true;
        query("set names utf8");
        return 0;
}

string PMySQL::get_project_content_by_code(string code)
{
       string sql       = string("SELECT `Content` FROM `ProjectContent` WHERE ProjectCode='")+code+string("'");
       return (query(sql)==0)?get_first_data():"";

}
void    PMySQL::set_number_invail(string code)
{
        string sql      = string("update Load_data set isUsed='1' where BaseCode='")+code+string("'");
        query(sql);
}
int PMySQL::add_project_content_by_code(string code, string content)
{
        bool isExist     = false;
        MYSQL_RES *res;
        MYSQL_ROW row;

        string sql    = string("select ProjectCode from ProjectContent where ProjectCode='")+code+string("' limit 1");
        query(sql);

        res = mysql_use_result(&m_mysql);
        if(res){
                for(int i=0;i<mysql_field_count(&m_mysql);i++){
                        row = mysql_fetch_row(res);
                        if(row)
                                isExist    = true;
                }
        }
        mysql_free_result(res);

        content     = replace_all_distinct(content, "'", "\\'");
        content     = replace_all_distinct(content, "\\", "\\\\'");
        if(isExist==true)
                sql     = string("update   `ProjectContent` set `Content`=CONCAT(`Content`, \'")+content+string("\') where ProjectCode=\'")+code+string("\'");
        else
                sql     = string("INSERT INTO `ProjectContent`(`ProjectCode`, `Content`) VALUES (\'")+code+string("\', \'")+content+string("\')");
        return query(sql);
}
string PMySQL::get_task_content_by_code(string code)
{
       string sql       = string("SELECT `Content` FROM `TaskContent` WHERE TaskCode=\"")+code+string("\"");
       return (query(sql)==0)?get_first_data():"";
}
int PMySQL::add_task_conent_by_code(string code, string content)
{
        return 0;
}

// 根据编码获取操作类型和操作代码
// 编码正确且未过期，返回操作代码，pkind为操作类型，如果pkind为-1则代表操作代码不正确或过期
// 操作类型：0代表项目、1代表任务
string PMySQL::get_action_by_number(string number, int* pkind)
{
        MYSQL_RES *res;
        MYSQL_ROW row;
        string output;
        int i, j;

        *pkind   = -1;
        output  = "";

        string sql      = string("SELECT  `Kind` ,  `ProjectCode` ,  `TaskCode` FROM  `Load_data` WHERE BaseCode='")+number+
                        string("' and isUsed='0' and (TIMESTAMPDIFF(MINUTE , `Time`,now())<3)");
        if(query(sql)<0)
                return output;

        res = mysql_use_result(&m_mysql);
        if(!res)
                return output;

        for(i=0;i<mysql_field_count(&m_mysql);i++){
                row = mysql_fetch_row(res);
                if(mysql_num_fields(res)!=0){
                        *pkind  = atoi(row[0]);
                        if(*pkind==0||*pkind==1)
                                output  = string(row[1]);
                        else if(*pkind==2||*pkind==3)
                                output  = string(row[2]);
                        else
                                *pkind  = -1;
                }
                break;
        }
        mysql_free_result(res);
        return output;
}

string PMySQL::get_first_data()
{
        int i, j;
        MYSQL_RES *res;
        MYSQL_ROW row;
        string output;

        if(isConnect==false)
                return "";
        res = mysql_use_result(&m_mysql);
        if(!res)
                return "";
        for(i=0;i<mysql_field_count(&m_mysql);i++){
                row = mysql_fetch_row(res);
                for(j=0;j<mysql_num_fields(res);j++){
                        printf("%s\n", output.c_str());
                        output  = string(row[j]);
                        break;
                }
                break;
        }
        mysql_free_result(res);
        return output;

}

// 成功返回0，否则返回>0
int PMySQL::query(string sql)
{
        if(isConnect==false)
                return -1;

        if(mysql_query(&m_mysql, sql.c_str())!=0)
                printf(sql.c_str());
        return 0;
}
