using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Office.Interop.OneNote;
using System.Xml.Linq;
using System.Windows.Forms;

namespace NoteSync
{
        // NoteBook section page结构
        struct Page{
                public string ID;
                public string name;
                public string lastModifiedTime;
                public string createTime;
                public string pageLevel;
        }
        struct Section{
                public string name;
                public string ID;
                public string lastModifiedTime;
                public List<Page> page;
        }
        struct NoteBook{
                public string name;
                public string nickname;
                public string ID;
                public string lastmodifiedtime;
                public List<Section> section;
        }

        // page内容结构
        struct OE{
                public string objectID;
                public string lastModifiedTime;
                public string creationTime;
                public string xml;
        }
        struct OEChildren{
                public List<OE> element;
        }
        struct outline{
                public string lastModifiedTime;
                public string objectID;
                public List<OEChildren> element;
        }

        struct Title{
                // lastModifiedBy="TSH" lastModifiedByInitials="T" creationTime="2015-03-30T01:14:40.000Z" lastModifiedTime="2015-03-30T01:14:40.000Z" objectID="{5D4B73A9-F9DA-4059-9FA5-E4A7087EB424}{15}{B0}" alignment="left"
                public string lastModifiedByInitials;
                public string creationTime;
                public string lastModifiedTime;
                public string xml;
        }

        struct PageContent{
                public Page page_header;
                public List<outline> element;
                public Title title;
        }

        class OneNoteOperation
        {
                private Microsoft.Office.Interop.OneNote.Application m_OneNoteApp;       // OneNote应用程序
                private XDocument m_OneNote_XML;        // 字符串转XML实例
                private XNamespace m_namespace;         // OneNote Name Space ，用得还是挺多的
                                
                public OneNoteOperation()
                {
                        // 构造函数，初始化Application
                        m_OneNoteApp = null;
                        m_OneNote_XML = null;
                        string onenote_xml;

                        try{
                                m_OneNoteApp = new Microsoft.Office.Interop.OneNote.Application();
                                m_OneNoteApp.GetHierarchy(null, HierarchyScope.hsPages, out onenote_xml);
                                m_OneNote_XML = XDocument.Parse(onenote_xml);
                                m_namespace = m_OneNote_XML.Root.Name.NamespaceName;
                        }catch{
                                MessageBox.Show("没有安装OneNote吗？还是安装一下吧。");
                                m_OneNoteApp = null;
                                m_OneNote_XML = null;
                        }
                }

                private void update_xml()
                {
                        string onenote_xml;
                        m_OneNoteApp.GetHierarchy(null, HierarchyScope.hsPages, out onenote_xml);
                        m_OneNote_XML = XDocument.Parse(onenote_xml);
                        m_namespace = m_OneNote_XML.Root.Name.NamespaceName;
                }

                // 检测是否安装OneNote软件，返回true则代表已经安装，反之代表未安装
                public bool isInstallOneNote()
                {
                        return (m_OneNoteApp == null) ? false : true;
                }

                // 获取所有的Notebook、Section、Page
                public List<NoteBook> get_all_page()
                {
                        update_xml();
                        List<NoteBook> output = new List<NoteBook>();
                        // 第一层循环是找到Notebook
                        foreach (var notebook in from node in m_OneNote_XML.Descendants(m_namespace + "Notebook") select node){
                                NoteBook notebookinfo = new NoteBook();
                                notebookinfo.section = new List<Section>();

                                notebookinfo.ID = notebook.Attribute("ID").Value;
                                notebookinfo.lastmodifiedtime = notebook.Attribute("lastModifiedTime").Value;
                                notebookinfo.name = notebook.Attribute("name").Value;
                                notebookinfo.nickname = notebook.Attribute("nickname").Value;

                                // 第二层循环找到Section
                                foreach (var section in from node in notebook.Descendants(m_namespace + "Section") select node){
                                        Section pSection = new Section();
                                        pSection.page = new List<Page>();
                                        pSection.ID = section.Attribute("ID").Value;
                                        pSection.lastModifiedTime = section.Attribute("lastModifiedTime").Value;
                                        pSection.name = section.Attribute("name").Value;

                                        // 第三层循环找到page
                                        foreach (var page in from node in section.Descendants(m_namespace + "Page") select node){
                                                Page pPage = new Page();
                                                pPage.createTime = page.Attribute("dateTime").Value;
                                                pPage.ID = page.Attribute("ID").Value;
                                                pPage.lastModifiedTime = page.Attribute("lastModifiedTime").Value;
                                                pPage.name = page.Attribute("name").Value;
                                                pPage.pageLevel = page.Attribute("pageLevel").Value;
                                                pSection.page.Add(pPage);
                                        }
                                        notebookinfo.section.Add(pSection);
                                }
                                output.Add(notebookinfo);
                        }
                        return output;
                }

                // 获取指定page的XML内容
                public string get_page_content_by_id(string id)
                {
                        string output   = "";
                        m_OneNoteApp.GetPageContent(id, out output, Microsoft.Office.Interop.OneNote.PageInfo.piBinaryData);
                        return output;
                }

                // 设置指定page的标题
                public void set_page_title(string page_id, string title)
                {

                }

                // 在指定page上插入新的outline，返回outline ID
                public string add_outline(string page_id)
                {
                        return "";
                }

                // 在指定outline插入OEChildren , 返回其ID
                public string add_OEchildren(string page_id, string outline_id)
                {
                        return "";
                }

                // 在指定的OEchildren增加OE， 返回其ID
                public string add_OE(string page_id, string outline_id, string children_id)
                {
                        return "";
                }

                // 更新指定OE内容
                public void update_OE(string page_id, string outline_id, string children_id, string OE_id, string value)
                {

                }

                ~OneNoteOperation()
                {
                }
        }
}
