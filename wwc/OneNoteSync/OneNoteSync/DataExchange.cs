using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace OneNoteSync
{
        class DataExchange
        {
                private string m_send_data;
                private string m_host;
                private string m_number;
                private int m_port;

                public DataExchange(string data, string ip, int port, string number)
                {
                        m_send_data = data;
                        m_host = ip;
                        m_number = number;
                        m_port = port;
                }
                public DataExchange(string ip, int port, string number)
                {
                }

                public void Send_Content()
                {
                        Socket ClientConnect = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                        IPEndPoint ServerInfo = new IPEndPoint(IPAddress.Parse(m_host), m_port);
                        byte[]  buffer=new byte[32];
                        byte[] send_byte = System.Text.Encoding.Default.GetBytes(m_send_data);
                        int length = send_byte.Length;

                        try {
                                ClientConnect.Connect(ServerInfo);
                        } catch {
                                return;
                        }

                       

                        // 传给服务端 编码和 数据大小
                        ClientConnect.Send(System.Text.Encoding.Default.GetBytes(m_number));
                        ClientConnect.Send(System.BitConverter.GetBytes(length));

                        try{
                                // 接收数据
                                ClientConnect.Receive(buffer);
                                string get_data = System.Text.Encoding.Default.GetString(buffer);
                                if (get_data.Equals(m_number)){
                                        ClientConnect.Send(send_byte);
                                }
                                ClientConnect.Receive(buffer);
                        } catch (SocketException e){
                                MessageBox.Show(e.Message);
                        }
                        ClientConnect.Shutdown(SocketShutdown.Both);
                        ClientConnect.Close();
                }
                public void Recv_Content()
                {

                }
        }
}
