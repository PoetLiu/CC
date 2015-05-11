#include "mysql.h"
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
        PMySQL mysqlcon;
        int     ret;

        if(mysqlcon.connect_mysql("localhost", "root", "weiwenchao", "Exon", 0)<0)
                return -1;

        mysqlcon.add_project_content_by_code("Exon", "askdfljasdf");
}
