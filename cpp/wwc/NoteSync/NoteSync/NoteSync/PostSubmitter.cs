using System;
using System.Text;
using System.IO;
using System.Web;
using System.Net;
using System.Collections.Specialized;

namespace NoteSync
{
        /// Submits post data to a url.
        public class PostSubmitter
        {
                /// determines what type of post to perform.
                public enum PostTypeEnum
                {
                        /// Does a get against the source.
                        Get,
                        /// Does a post against the source.
                        Post
                }

                private string m_url = string.Empty;
                private NameValueCollection m_values = new NameValueCollection();
                private PostTypeEnum m_type = PostTypeEnum.Get;
                /// Default constructor.
                public PostSubmitter()
                {
                }

                /// Constructor that accepts a url as a parameter
                /// <param name="url">The url where the post will be submitted to.</param>
                public PostSubmitter(string url) : this()
                {
                        m_url = url;
                }

                /// Constructor allowing the setting of the url and items to post.
                /// <param name="url">the url for the post.</param>
                /// <param name="values">The values for the post.</param>
                public PostSubmitter(string url, NameValueCollection values) : this(url)
                {
                        m_values = values;
                }

                /// Gets or sets the url to submit the post to.
                public string Url{
                        get{
                                return m_url;
                        }
                        set{
                                m_url = value;
                        }
                }
                /// Gets or sets the name value collection of items to post.
                public NameValueCollection PostItems{
                        get{
                                return m_values;
                        }
                        set{
                                m_values = value;
                        }
                }
                /// Gets or sets the type of action to perform against the url.
                public PostTypeEnum Type
                {
                        get{
                                return m_type;
                        }
                        set{
                                m_type = value;
                        }
                }
                /// Posts the supplied data to specified url.
                /// <returns>a string containing the result of the post.</returns>
                public string Post()
                {
                        StringBuilder parameters = new StringBuilder();
                        for (int i = 0; i < m_values.Count; i++){
                                EncodeAndAddItem(ref parameters, m_values.GetKey(i), m_values[i]);
                        }
                        string result = PostData(m_url, parameters.ToString());
                        return result;
                }
                /// Posts the supplied data to specified url.
                /// <param name="url">The url to post to.</param>
                /// <returns>a string containing the result of the post.</returns>
                public string Post(string url)
                {
                        m_url = url;
                        return this.Post();
                }
                /// Posts the supplied data to specified url.
                /// <param name="url">The url to post to.</param>
                /// <param name="values">The values to post.</param>
                /// <returns>a string containing the result of the post.</returns>
                public string Post(string url, NameValueCollection values)
                {
                        m_values = values;
                        return this.Post(url);
                }
                /// Posts data to a specified url. Note that this assumes that you have already url encoded the post data.
                /// <param name="postData">The data to post.</param>
                /// <param name="url">the url to post to.</param>
                /// <returns>Returns the result of the post.</returns>
                private string PostData(string url, string postData)
                {
                        HttpWebRequest request = null;
                        System.Net.ServicePointManager.DefaultConnectionLimit = 50;
                        if (m_type == PostTypeEnum.Post){
                                Uri uri = new Uri(url);
                                request = (HttpWebRequest)WebRequest.Create(uri);
                                request.Proxy = null;
                                request.Method = "POST";
                                request.ContentType = "application/x-www-form-urlencoded";
                                request.ContentLength = postData.Length;
                                using (Stream writeStream = request.GetRequestStream()){
                                        UTF8Encoding encoding = new UTF8Encoding();
                                        byte[] bytes = encoding.GetBytes(postData);
                                        writeStream.Write(bytes, 0, bytes.Length);
                                }
                        }else{
                                Uri uri = new Uri(url);
                                request = (HttpWebRequest)WebRequest.Create(uri);
                                request.Proxy = null;
                                request.Method = "GET";
                        }
                        string result = string.Empty;
                        using (HttpWebResponse response = (HttpWebResponse)request.GetResponse()){
                                using (Stream responseStream = response.GetResponseStream()){
                                        using (StreamReader readStream = new StreamReader(responseStream, Encoding.UTF8)){
                                                result = readStream.ReadToEnd();
                                        }
                                }
                        }
                        return result;
                }
                /// Encodes an item and ads it to the string.
                /// <param name="baseRequest">The previously encoded data.</param>
                /// <param name="dataItem">The data to encode.</param>
                /// <returns>A string containing the old data and the previously encoded data.</returns>
                private void EncodeAndAddItem(ref StringBuilder baseRequest, string key, string dataItem)
                {
                        if (baseRequest == null){
                                baseRequest = new StringBuilder();
                        }
                        if (baseRequest.Length != 0){
                                baseRequest.Append("&");
                        }
                        baseRequest.Append(key);
                        baseRequest.Append("=");
                        baseRequest.Append(System.Web.HttpUtility.UrlEncode(dataItem));
                }
        }
}
