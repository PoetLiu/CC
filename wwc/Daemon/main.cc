#include <stdio.h>      // for printf
#include <string>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mysql.h"

using namespace std;

#define PORT            3563
#define LISTEN_NUMBER   1024
#define READ_BUFFER     (4096*2)

#define MYSQL_HOST            "localhost"
#define MYSQL_USER            "root"
#define MYSQL_PASSWD          "weiwenchao"
#define MYSQL_DATABASE        "Exon"


#define DEBUG
#ifdef DEBUG
#define output_log              printf
#else
#define output_log        ;
#endif

void* Processing_Connection(void *fd)
{
        int     cfd;
        int     ret;
        char    header[37];             // 第一次发的数据是36字节
        int     data_size;              // 数据大小
        PMySQL  *MySQLCon;
        int     kind;
        string  code;
        int     data_count  = 0;
        char    buffer[READ_BUFFER];

        cfd     = *(int*)fd;
        MySQLCon       = NULL;
        printf("\n***%d***\n", cfd);

         // recv data
        ret     = recv(cfd, header, 36, 0);
        if(ret<=0){
                goto finish;
        }

        data_size       = *(int*)(header+32);
        header[32]      = 0;

        MySQLCon        = new PMySQL();
        if(!MySQLCon)
                goto finish;

        // MySQL 认证
        if(MySQLCon->connect_mysql(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD, MYSQL_DATABASE, 0)<0){
                printf("Connect mysql error");
                goto finish;
        }

        // 检测编码
        code    = MySQLCon->get_action_by_number(string(header), &kind);
        if(kind<0)
                goto finish;

        switch(kind){
                case 0: {// 项目上载：发送回编码，开始接收数据，每次接收1024字节
                        ret     = send(cfd, header, 32, 0);
                        while(data_count<data_size){
                                ret     = recv(cfd, buffer, (data_count+READ_BUFFER-1<data_size)?READ_BUFFER-1:data_size-data_count, 0);
                                if(ret<=0){
                                        goto finish;
                                }
                                buffer[ret]     = 0;
                                MySQLCon->add_project_content_by_code(code, string(buffer));
                                data_count+=ret;
                        }
                        ret     = send(cfd, header, 32, 0);
                        }break;
                case 1: // 项目下载
                        break;
                case 2: // 任务上载
                        break;
                case 3: // 任务下载
                        break;
                default:
                        goto finish;
        }
finish:
        MySQLCon->set_number_invail(header);
        if(MySQLCon)
                delete MySQLCon;
        printf("\nend##LRi%d**\n", cfd);
        close(cfd);
        return NULL;
}

int server_listen(int argc, char* argv[])
{
       int                      sfd, cfd;                       // server file descriptor, and client file descriptor
       struct  sockaddr_in      s_add,c_add;                    // server address and client address
       int                      sin_size;                       // sockaddr_in size 
       int                      port;
       pthread_t                pid;

       port     = PORT;
       sin_size = sizeof(struct sockaddr_in);

       sfd = socket(AF_INET, SOCK_STREAM, 0);
       if(sfd==-1){
               output_log("failed to create socket;\n");
               return -1;              
       }
       
       s_add.sin_family=AF_INET;
       s_add.sin_addr.s_addr=htonl(INADDR_ANY);
       s_add.sin_port=htons(port);
       if(bind(sfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr))==-1){
               output_log("bind fail !\r\n");
               return -2;
       }

       if(listen(sfd,LISTEN_NUMBER)==-1){
               output_log("listen fail !\r\n");
               return -3;               
       }

       output_log("listen port:%d\n", port);
       while(1){
               cfd = accept(sfd, (struct sockaddr *)(&c_add), (unsigned int*)&sin_size);
               if(cfd==-1){
                       output_log("accept fail !\r\n");
                       return -3;
               }
               pthread_create(&pid, NULL, Processing_Connection, &cfd);
       }
       close(sfd);

       return 0;
}


int main(int argc, char* argv[])
{
        // todo Daemon initialization

        return server_listen(argc, argv);
}
