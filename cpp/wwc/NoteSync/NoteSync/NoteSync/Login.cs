using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.Serialization.Json;
using System.Runtime.Serialization.Formatters.Binary;
using System.Web.Script.Serialization;
using System.Runtime;

namespace NoteSync
{
        public class userinfo
        {
                public string auth { get; set; }			//要和JSON数据中的字符匹配，否则取出来是为空的
                public string name { get; set; }
                public string user { get; set; }
        }

        public partial class Login : Form
        {
                private PostSubmitter m_post      = null;
                private string m_login_url = "http://192.168.248.1/OneNoteSync/interface.php?Login";
                private JavaScriptSerializer m_json=null;
                public List<userinfo> m_userinfo;
                public string m_passwd;
                public Login()
                {
                        m_post = new PostSubmitter();
                        m_json = new JavaScriptSerializer();
                        InitializeComponent();
                }

                private void LoginButton_Click(object sender, EventArgs e)
                {
                        string result;
                        if (user_text.Text == "" || passwd_text.Text == ""){
                                MessageBox.Show("请输入用户名和密码");
                        }else{
                                m_post.PostItems.Clear();
                                m_post.Url = m_login_url;
                                m_post.PostItems.Add("user", user_text.Text);
                                m_post.PostItems.Add("passwd", passwd_text.Text);
                                m_post.Type = PostSubmitter.PostTypeEnum.Post;
                                result = m_post.Post();
                                result = "[" + result + "]";
                                m_userinfo      = m_json.Deserialize<List<userinfo>>(result);
                                if (m_userinfo[0].auth != "true"){
                                        MessageBox.Show("用户名或密码错误");
                                }else{
                                        m_passwd = passwd_text.Text;
                                        this.DialogResult = DialogResult.OK;
                                }
                        }
                }
        }
}
