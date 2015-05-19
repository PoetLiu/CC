using System.Windows.Forms;
namespace NoteSync
{
        partial class main
        {
                /// <summary>
                /// Required designer variable.
                /// </summary>
                private System.ComponentModel.IContainer components = null;

                /// <summary>
                /// Clean up any resources being used.
                /// </summary>
                /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
                protected override void Dispose(bool disposing)
                {
                        if (disposing && (components != null)){
                                components.Dispose();
                        }
                        base.Dispose(disposing);
                }

                #region Windows Form Designer generated code

                /// <summary>
                /// Required method for Designer support - do not modify
                /// the contents of this method with the code editor.
                /// </summary>
                private void InitializeComponent()
                {
                        this.LocalOneNoteNode = new System.Windows.Forms.TreeView();
                        this.sync = new System.Windows.Forms.Button();
                        this.Login = new System.Windows.Forms.Button();
                        this.ServerOneNoteNode = new System.Windows.Forms.TreeView();
                        this.LocalLabel = new System.Windows.Forms.Label();
                        this.ServerNode = new System.Windows.Forms.Label();
                        this.RefreshServerNode = new System.Windows.Forms.Button();
                        this.RefreshLocalNode = new System.Windows.Forms.Button();
                        this.SyncToLocal = new System.Windows.Forms.Button();
                        this.SuspendLayout();
                        // 
                        // LocalOneNoteNode
                        // 
                        this.LocalOneNoteNode.CheckBoxes = true;
                        this.LocalOneNoteNode.Location = new System.Drawing.Point(13, 34);
                        this.LocalOneNoteNode.Name = "LocalOneNoteNode";
                        this.LocalOneNoteNode.Size = new System.Drawing.Size(313, 355);
                        this.LocalOneNoteNode.TabIndex = 0;
                        this.LocalOneNoteNode.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.OneNoteNode_AfterSelect);
                        // 
                        // sync
                        // 
                        this.sync.Location = new System.Drawing.Point(221, 8);
                        this.sync.Name = "sync";
                        this.sync.Size = new System.Drawing.Size(86, 23);
                        this.sync.TabIndex = 1;
                        this.sync.Text = "同步到服务器";
                        this.sync.UseVisualStyleBackColor = true;
                        this.sync.Click += new System.EventHandler(this.sync_Click);
                        // 
                        // Login
                        // 
                        this.Login.Location = new System.Drawing.Point(656, 74);
                        this.Login.Name = "Login";
                        this.Login.Size = new System.Drawing.Size(75, 23);
                        this.Login.TabIndex = 1;
                        this.Login.Text = "登录";
                        this.Login.UseVisualStyleBackColor = true;
                        this.Login.Click += new System.EventHandler(this.Login_Click);
                        // 
                        // ServerOneNoteNode
                        // 
                        this.ServerOneNoteNode.Location = new System.Drawing.Point(341, 34);
                        this.ServerOneNoteNode.Name = "ServerOneNoteNode";
                        this.ServerOneNoteNode.Size = new System.Drawing.Size(309, 355);
                        this.ServerOneNoteNode.TabIndex = 2;
                        this.ServerOneNoteNode.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.ServerOneNoteNode_AfterSelect);
                        this.ServerOneNoteNode.CheckBoxes = true;
                        // 
                        // LocalLabel
                        // 
                        this.LocalLabel.AutoSize = true;
                        this.LocalLabel.Location = new System.Drawing.Point(13, 13);
                        this.LocalLabel.Name = "LocalLabel";
                        this.LocalLabel.Size = new System.Drawing.Size(95, 12);
                        this.LocalLabel.TabIndex = 3;
                        this.LocalLabel.Text = "本地OneNote节点";
                        // 
                        // ServerNode
                        // 
                        this.ServerNode.AutoSize = true;
                        this.ServerNode.Location = new System.Drawing.Point(339, 9);
                        this.ServerNode.Name = "ServerNode";
                        this.ServerNode.Size = new System.Drawing.Size(65, 12);
                        this.ServerNode.TabIndex = 3;
                        this.ServerNode.Text = "服务端节点";
                        // 
                        // RefreshServerNode
                        // 
                        this.RefreshServerNode.Location = new System.Drawing.Point(423, 9);
                        this.RefreshServerNode.Name = "RefreshServerNode";
                        this.RefreshServerNode.Size = new System.Drawing.Size(75, 23);
                        this.RefreshServerNode.TabIndex = 1;
                        this.RefreshServerNode.Text = "刷新";
                        this.RefreshServerNode.UseVisualStyleBackColor = true;
                        this.RefreshServerNode.Click += new System.EventHandler(this.RefreshServerNode_Click);
                        // 
                        // RefreshLocalNode
                        // 
                        this.RefreshLocalNode.Location = new System.Drawing.Point(129, 8);
                        this.RefreshLocalNode.Name = "RefreshLocalNode";
                        this.RefreshLocalNode.Size = new System.Drawing.Size(75, 23);
                        this.RefreshLocalNode.TabIndex = 1;
                        this.RefreshLocalNode.Text = "刷新";
                        this.RefreshLocalNode.UseVisualStyleBackColor = true;
                        this.RefreshLocalNode.Click += new System.EventHandler(this.RefreshLocalNode_Click);
                        // 
                        // SyncToLocal
                        // 
                        this.SyncToLocal.Location = new System.Drawing.Point(526, 7);
                        this.SyncToLocal.Name = "SyncToLocal";
                        this.SyncToLocal.Size = new System.Drawing.Size(95, 23);
                        this.SyncToLocal.TabIndex = 4;
                        this.SyncToLocal.Text = "同步到本地";
                        this.SyncToLocal.UseVisualStyleBackColor = true;
                        // 
                        // main
                        // 
                        this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
                        this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
                        this.ClientSize = new System.Drawing.Size(743, 400);
                        this.Controls.Add(this.SyncToLocal);
                        this.Controls.Add(this.ServerNode);
                        this.Controls.Add(this.LocalLabel);
                        this.Controls.Add(this.ServerOneNoteNode);
                        this.Controls.Add(this.RefreshServerNode);
                        this.Controls.Add(this.RefreshLocalNode);
                        this.Controls.Add(this.Login);
                        this.Controls.Add(this.sync);
                        this.Controls.Add(this.LocalOneNoteNode);
                        this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
                        this.MaximizeBox = false;
                        this.MinimizeBox = false;
                        this.Name = "main";
                        this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
                        this.Text = "OneNoteSync";
                        this.ResumeLayout(false);
                        this.PerformLayout();

                }

                #endregion

                private System.Windows.Forms.TreeView LocalOneNoteNode;
                private System.Windows.Forms.Button sync;
                private Button Login;
                private TreeView ServerOneNoteNode;
                private Label LocalLabel;
                private Label ServerNode;
                private Button RefreshServerNode;
                private Button RefreshLocalNode;
                private Button SyncToLocal;
        }
}

