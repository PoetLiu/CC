using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
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
        public partial class main : Form
        {
                private OneNoteOperation m_OneNoteOp=null;
                private List<NoteBook> m_OneNote_Info=null;
                private List<userinfo> m_userinfo       = null;
                private PostSubmitter m_post = null;
                private JavaScriptSerializer m_json=null;
                private string m_passwd;

                private string m_syncurl = "http://192.168.248.1/OneNoteSync/interface.php?sync_notebook";
                private string m_get_listurl = "http://192.168.248.1/OneNoteSync/interface.php?get_all_list";

                public main()
                {
                        InitializeComponent();

                        // 初始化本地OneNote列表
                        m_OneNoteOp = new OneNoteOperation();
                        update_local_node();

                        m_post = new PostSubmitter();
                        m_json = new JavaScriptSerializer();
                }

                // 更新本地OneNote节点
                public void update_local_node()
                {
                        LocalOneNoteNode.Nodes.Clear();
                        if(m_OneNote_Info!=null)
                                m_OneNote_Info.Clear();
                        if (m_OneNoteOp != null){
                                m_OneNote_Info = m_OneNoteOp.get_all_page();

                                // 插入到treeview中
                                foreach (var notebook in m_OneNote_Info){
                                        TreeNode tree_notebook = new TreeNode(); ;
                                        tree_notebook.Name = notebook.ID;
                                        tree_notebook.Text = notebook.name;
                                        foreach (var section in notebook.section){
                                                TreeNode tree_section = new TreeNode();
                                                tree_section.Name = section.ID;
                                                tree_section.Text = section.name;
                                                foreach (var page in section.page){
                                                        TreeNode tree_page = new TreeNode();
                                                        tree_page.Name = page.ID;
                                                        tree_page.Text = page.name;
                                                        tree_section.Nodes.Add(tree_page);
                                                }
                                                tree_notebook.Nodes.Add(tree_section);

                                        }
                                        LocalOneNoteNode.Nodes.Add(tree_notebook);

                                }
                        }else{
                                MessageBox.Show("内存不够了吗？");
                        }
                }

                private void OneNoteNode_AfterSelect(object sender, TreeViewEventArgs e)
                {
                       //通过鼠标或者键盘触发事件，防止修改节点的Checked状态时候再次进入
                       if (e.Action == TreeViewAction.ByMouse || e.Action == TreeViewAction.ByKeyboard){
                               SetChildNodeCheckedState(e.Node, e.Node.Checked);
                               SetParentNodeCheckedState(e.Node, !e.Node.Checked);
                       }
                }

                //设置子节点状态
                private void SetChildNodeCheckedState(TreeNode currNode, bool isCheckedOrNot)
                {
                        currNode.Checked = isCheckedOrNot;
                        if (currNode.Nodes == null) return; //没有子节点返回
                        foreach (TreeNode tmpNode in currNode.Nodes){
                                tmpNode.Checked = isCheckedOrNot;
                                SetChildNodeCheckedState(tmpNode, isCheckedOrNot);
                        }
                }

                //设置父节点状态
                private void SetParentNodeCheckedState(TreeNode currNode, bool isCheckedOrNot)
                {
                        if (currNode.Parent == null) return; //没有父节点返回
                        if (isCheckedOrNot){
                                //如果当前节点被选中，则设置所有父节点都被选中
                                currNode.Parent.Checked = isCheckedOrNot;
                                SetParentNodeCheckedState(currNode.Parent, isCheckedOrNot);
                        }else{
                                //如果当前节点没有被选中，则当其父节点的子节点有一个被选中时，父节点被选中，否则父节点不被选中
                                bool checkedFlag = false;
                                foreach (TreeNode tmpNode in currNode.Parent.Nodes){
                                        if (tmpNode.Checked){
                                                checkedFlag = true;
                                                break;
                                        }
                                }
                                currNode.Parent.Checked = checkedFlag;
                                SetParentNodeCheckedState(currNode.Parent, checkedFlag);
                        }
                }

                private void sync_Click(object sender, EventArgs e)
                {
                        string result;
                        List<NoteBook> output   = new List<NoteBook>();
                        int i, j, k;
                        i = 0;
                        j = 0;
                        k = 0;
                        // 检测登录状况
                        if (m_userinfo == null ||! m_userinfo[0].auth.Equals("true")){
                                MessageBox.Show("未登录");
                                return;
                        }

                        // 获得被选中的Notebook、Section、page
                        foreach (TreeNode notebook in LocalOneNoteNode.Nodes){
                                NoteBook _notebook = new NoteBook();
                                _notebook.section       = new List<Section>();
                                bool isSelect = false;
                                j = 0;
                                foreach (TreeNode section in notebook.Nodes){
                                        Section _section = new Section();
                                        _section.page = new List<Page>();
                                        bool issselect = false;
                                        k = 0;
                                        foreach (TreeNode page in section.Nodes){
                                                Page _page = new Page();
                                                bool ispselect = false;
                                               if (page.Checked == true){
                                                       isSelect = true;
                                                       issselect = true;
                                                       ispselect = true; ;
                                               }
                                               if (ispselect == true){
                                                       _page.createTime = m_OneNote_Info[i].section[j].page[k].createTime;
                                                       _page.ID = m_OneNote_Info[i].section[j].page[k].ID;
                                                       _page.lastModifiedTime = m_OneNote_Info[i].section[j].page[k].lastModifiedTime;
                                                       _page.name = m_OneNote_Info[i].section[j].page[k].name;
                                                       _page.pageLevel = m_OneNote_Info[i].section[j].page[k].pageLevel;
                                                       _section.page.Add(_page);
                                               }
                                               k++;
                                        }
                                        if (issselect == true){
                                                _section.ID = m_OneNote_Info[i].section[j].ID;
                                                _section.lastModifiedTime = m_OneNote_Info[i].section[j].lastModifiedTime;
                                                _section.name = m_OneNote_Info[i].section[j].name;
                                                _notebook.section.Add(_section);

                                        }
                                        j++;
                               }
                                if (isSelect){
                                        _notebook.ID = m_OneNote_Info[i].ID;
                                        _notebook.lastmodifiedtime = m_OneNote_Info[i].lastmodifiedtime;
                                        _notebook.name = m_OneNote_Info[i].name;
                                        _notebook.nickname = m_OneNote_Info[i].nickname;
                                        output.Add(_notebook);
                                }
                               i++;
                        }
                        m_json.MaxJsonLength = 10240000;
                        result = m_json.Serialize(output);


                        // post 
                        m_post.PostItems.Clear();
                        m_post.Url = m_syncurl;
                        m_post.PostItems.Add("user", m_userinfo[0].user);
                        m_post.PostItems.Add("passwd", m_passwd);
                        m_post.PostItems.Add("content", result);
                        m_post.Type = PostSubmitter.PostTypeEnum.Post;
                        result = m_post.Post();
                }

                private void ServerOneNoteNode_AfterSelect(object sender, TreeViewEventArgs e)
                {
                       // 通过鼠标或者键盘触发事件，防止修改节点的Checked状态时候再次进入
                       if (e.Action == TreeViewAction.ByMouse || e.Action == TreeViewAction.ByKeyboard){
                               SetChildNodeCheckedState(e.Node, e.Node.Checked);
                               SetParentNodeCheckedState(e.Node, !e.Node.Checked);
                       }
                }

                private void RefreshServerNode_Click(object sender, EventArgs e)
                {
                        update_server_node();
                }

                private void update_server_node()
                {
                        string result;
                        List<NoteBook> output;
                        // 检测登录状况
                        if (m_userinfo == null ||! m_userinfo[0].auth.Equals("true")){
                                MessageBox.Show("未登录");
                                return;
                        }
                        m_post.Url = m_get_listurl;
                        m_post.PostItems.Clear();
                        m_post.PostItems.Add("user", m_userinfo[0].user);
                        m_post.PostItems.Add("passwd", m_passwd);
                        m_post.Type = PostSubmitter.PostTypeEnum.Post;
                        result  = m_post.Post();
                        output = m_json.Deserialize<List<NoteBook>>(result);

                        ServerOneNoteNode.Nodes.Clear();
                        foreach (NoteBook notebook in output){
                                TreeNode tree_notebook = new TreeNode();
                                tree_notebook.Text = notebook.name;
                                foreach(Section section in notebook.section){
                                        TreeNode tree_section = new TreeNode();
                                        tree_section.Text = section.name;
                                        foreach (Page page in section.page){
                                                TreeNode tree_page = new TreeNode();
                                                tree_page.Text = page.name;
                                                tree_section.Nodes.Add(tree_page);
                                        }
                                        tree_notebook.Nodes.Add(tree_section);
                                }
                                ServerOneNoteNode.Nodes.Add(tree_notebook);
                        }

                }

                private void RefreshLocalNode_Click(object sender, EventArgs e)
                {
                        update_local_node();
                }

                private void Login_Click(object sender, EventArgs e)
                {
                        if (Login.Text.Equals("退出")){
                                Login.Text = "登录";
                                m_userinfo = null;
                                m_passwd        = "";
                        }else{
                                Login pLoginInterface = new Login();
                                pLoginInterface.ShowDialog();
                                if (pLoginInterface.DialogResult != DialogResult.OK)
                                        return;
                                m_userinfo = pLoginInterface.m_userinfo;
                                if (m_userinfo[0].auth.Equals("true"))
                                        Login.Text = "退出";
                                m_passwd = pLoginInterface.m_passwd;

                        }
                }
        }
}
