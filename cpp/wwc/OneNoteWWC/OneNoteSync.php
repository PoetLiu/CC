<?php
/*
 * OneNote Sync PHP script
 * author       weiwenchao
 * time         2015-05-03
 * */
include "./MySQLConfig.php";
class OneNoteSync
{
        private $MySQLCon;
        private $UID;
        // function of construc : initial MySQL connect
        public function __construct()
        {
                $this->MySQLCon = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD);
                if(!$this->MySQLCon){
                        die('Could not connect: ' . mysql_error());
                }

                $db_selected = mysql_select_db(MYSQL_NAME, $this->MySQLCon);
                if (!$db_selected){
                          die ("Can\'t use database  : " . mysql_error());
                }

                mysql_query("set names utf8");

        }

        private function auth($user, $passwd)
        {
                $isRight       = false;
                $sql    = "select ID from User where user='$user' and passwd='$passwd' limit 1";
                $result = mysql_query($sql);
                while($row=mysql_fetch_row($result, MYSQL_ASSOC)){
                        $isRight        = true;
                        $this->UID = $row["ID"];
                        break;
                }
                return $isRight;
        }

        public function Login()
        {
                $user   = $_POST['user'];
                $passwd = $_POST['passwd'];
                $output = array();

                $output ['auth']        = "";
                $output ['name']        = "";
                $output ['user']        = $user;

                // 认证
                if(!isset($user)||!isset($passwd))
                        $output['auth'] = 'false';
                else if($this->auth($user, $passwd)==true)
                        $output['auth'] = 'true';
                else
                        $output['auth'] = 'false';

                // 获取信息
                if($output['auth']=='true'){
                        $result = mysql_query("select name from User where user='$user'");
                        while($row=mysql_fetch_row($result, MYSQL_ASSOC)){
                                $output ['name']        = $row['name'];
                                break;
                        }
                }
                echo json_encode($output,JSON_UNESCAPED_UNICODE);
        }

        // 检测是否存在Notebook，不存在则插入，返回NotebookID
        private function add_Notebook($notebook_name, $nID, $uID, $lastmodifiedtime)
        {
                $sql    = "select * from Notebook where nID='$nID' and uID='$uID'";
                $ID;
                $isHaveNoteBook = false;
                $Dblastmodifiedtime; 
                $result = mysql_query($sql);

                // 存在则比对最后修改时间，如果修改时间更新则更新
                while($row=mysql_fetch_row($result, MYSQL_ASSOC)){
                        $Dblastmodifiedtime     = $row["lastmodifiedtime"];
                        $isHaveNoteBook = true;
                        $ID     = $row["ID"];
                        break;
                }
                if($isHaveNoteBook==true){
                        if($this->cacu_time($Dblastmodifiedtime, $temp_time)==true){
                                // 更新数据库
                                $sql    = "update Notebook set name='$notebook_name',lastmodifiedtime='$lastmodifiedtime' where nID='$nID' and uID='$uID'";
                                mysql_query($sql);
                        }
                }else{
                        // 无数据则插入
                        $sql    = "INSERT INTO `Notebook`(`name`, `nID`, `uID`, `lastmodifiedtime`) VALUES('$notebook_name', '$nID', '$uID', '$lastmodifiedtime');";
			$result	= mysql_query($sql);
                        $ID     = mysql_insert_id();
                }
                return $ID;
        }

        // 返回真则time1比time2早
        public function cacu_time($time1, $time2)
        {
                $time1      = substr($time1, 0, 19);
                str_replace("T", " ", $time1);
                $time2     = substr($time2, 0, 19);
                str_replace("T", " ", $time2);
                return strtotime($time1)<strtotime($time2)?true:false;
        }

        // 增加Section,如果存在则更新，返回SectionID
        private function add_Section($nID, $section_ID, $section_name, $section_lastModifiedTime)
        {
                $sql    = "select * from Section where sID='$section_ID' and nID='$nID'";
                $ID;
                $lastModifytime;
                $result = mysql_query($sql);
                $isHave = false;
                while($row=mysql_fetch_row($result, MYSQL_ASSOC)){
                        $isHave = true;
                        $ID     = $row["ID"];
                        $lastModifytime = $row["lastModifiedTime"];
                        break;
                }

                if($isHave==true){
                        // 更新数据
                        if($this->cacu_time($lastModifytime, $section_lastModifiedTime)==true){
                                $sql    = "update Section set name='$section_name',lastModifiedTime='$section_lastModifiedTime' where sID='$section_ID' and nID='$nID'";
                                mysql_query($sql);
                        }
                }else{
                        // 插入数据
                        $sql    = "INSERT INTO `Section`(`name`, `sID`, `nID`, `lastModifiedTime`) VALUES('$section_name', '$section_ID', '$nID', '$section_lastModifiedTime');";
			$result	= mysql_query($sql);
                        $ID     = mysql_insert_id();
                }
                return $ID;
        }

	private function get_client_IP()
	{
		$user_IP = ($_SERVER["HTTP_VIA"]) ? $_SERVER["HTTP_X_FORWARDED_FOR"] : $_SERVER["REMOTE_ADDR"];
		$user_IP = ($user_IP) ? $user_IP : $_SERVER["REMOTE_ADDR"];
		return $user_IP;
	}

	// 插入同步记录,返回ID
	private function add_client_sync($number)
	{
		// 获取IP, 当前时间
		$clientIP	= $this->get_client_IP();
		$time		= date("Y-m-d h:i:s");

		$sql		= "INSERT INTO `SyncClient`(`Number`, `ClientIP`, `Time`) VALUES ('$number', '$clientIP', '$time');";
		$result	= mysql_query($sql);
                return mysql_insert_id();
	}

        //  增加page，如果不存在则创建，返回ID;$Status: 0 不插入也不更新，1 插入，2 更新
        private function add_page($page_name, $page_ID, $page_lastModifiedTime, $page_pageLevel, $sID, &$Status)
        {
                $sql    = "select * from Page where pID='$page_ID' and sID='$sID'";
                $result = mysql_query($sql);
                $isHave = false;
                $ID;
                $lastModifytime;
                while($row=mysql_fetch_row($result, MYSQL_ASSOC)){
                        $isHave = true;
                        $ID     = $row["ID"];
                        $lastModifytime = $row["lastModifiedTime"];
                        break;
                }

                echo "***$lastModifytime:$page_lastModifiedTime***";
                if($isHave==true){
                        // 更新数据
                        if($this->cacu_time($lastModifytime, $page_lastModifiedTime)==true){
                                $sql    = "update Page set name='$page_name',lastModifiedTime='$page_lastModifiedTime' where pID='$page_ID' and sID='$sID'";
                                mysql_query($sql);
                                $Status = 2;
                        }else
                                $Status = 0;
                }else{
                        // 插入数据
                        $sql    = "INSERT INTO `Page`(`name`, `pID`, `sID`, `lastModifiedTime`, `Content`) VALUES ('$page_name', '$page_ID', '$sID', '$page_lastModifiedTime', '');";
			$result = mysql_query($sql);
                        $ID     = mysql_insert_id();
                        $Status = 1;
                }
                return $ID;
        }

	public function getRandChar($length)
	{
		$str = null;
		$strPol = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789abcdefghijklmnopqrstuvwxyz";
		$max = strlen($strPol)-1;
		for($i=0;$i<$length;$i++)
			$str.=$strPol[rand(0,$max)];
		return $str;
	}

        // $pageID      page 表的ID
        // $pID         页面的ID(XML )
        // 返回ID
        private function add_sync_page($syncID, $pageID, $pID, $kind)
        {
                $sql    = "INSERT INTO `SyncPage`(`SyncClientID`, `PageID`, `pID`, `Kind`, `IsUse`) VALUES ('$syncID', '$pageID', '$pID', '$kind', '1')";
                echo $sql;
                mysql_query($sql);
                return mysql_insert_id();

        }
	// 本地同步到服务器的函数
	// 1. 用户认证；
	// 2. 生成32位的随机编码；
	// 3. 解析数据，检测哪些page需要更新的 
        public function sync_notebook()
        {

                $user   = $_POST['user'];
                $passwd = $_POST['passwd'];
                $output = array();
                $output ['auth']        = "false";
                $output ['sync']        = "false";
                $output ['host']        = "";
                $output ['port']        = "";
                $output ['number']      = "";
                if(!isset($user))
                        return;
                if($this->auth($user, $passwd)==false){
                        echo json_encode($output,JSON_UNESCAPED_UNICODE);
                        return;
                }
                
		// 生成32位编码
		$output ['number']	= $this->getRandChar(32);
                $syncID = $this->add_client_sync($output ['number']);

                $output ['auth']        = "true";
                $data   = json_decode($_POST['content']);
                if(!$data){
                        $output ['sync']        = "false";
                }else{
                        for($i=0;$i<count($data);$i++){
                                $notebook_name  = $data[$i]->name;
                                $notebook_nickname      = $data[$i]->nickname;
                                $notebook_ID            = $data[$i]->ID;
                                $notebook_lastmodifiedtime      = $data[$i]->lastmodifiedtime;

                                $nID    = $this->add_Notebook($notebook_name, $notebook_ID, $this->UID, $notebook_lastmodifiedtime);

                                $section        = $data[$i]->section;
                                for($j=0;$j<count($section);$j++){
                                        $section_ID     = $section[$j]->ID;
                                        $section_name   = $section[$j]->name;
                                        $section_lastModifiedTime       = $section[$j]->lastModifiedTime;

                                        $sID    = $this->add_Section($nID, $section_ID, $section_name, $section_lastModifiedTime);

                                        $page   = $section[$j]->page;
                                        for($k=0;$k<count($page);$k++){
                                                $page_ID        = $page[$k]->ID;
                                                $page_name      = $page[$k]->name;
                                                $page_lastModifiedTime  = $page[$k]->lastModifiedTime;
                                                $page_createTime        = $page[$k]->createTime;
                                                $page_pageLevel         = $page[$k]->pageLevel;

                                                $Status = -1;
                                                $pageID = $this->add_page($page_name, $page_ID, $page_lastModifiedTime, $page_pageLevel, $sID, $Status);
                                                echo "**".$Status."##";
                                                if($Status!=0&&$Status!=-1){
                                                        if($Status==1)  //  插入
                                                                $this->add_sync_page($syncID, $pageID, $page_ID, "0");
                                                        else if($Status==2)     // 更新
                                                                $this->add_sync_page($syncID, $pageID, $page_ID, "1");
                                                }
                                        }
                                }
                        }
                        $output ['sync']        = "true";
                }
                echo json_encode($output,JSON_UNESCAPED_UNICODE);
        }

	// 返回服务器所有的列表 
        public function get_all_list()
        {
                $user   = $_POST['user'];
                $passwd = $_POST['passwd'];
                $output = array();
                if(!isset($user))
                        return;
                if($this->auth($user, $passwd)==false){
                        return;
                }
                $UID    = $this->UID;
                $sql    = "select ID, name,nID,lastmodifiedtime from Notebook where uID='$UID'";
                $notebook_result        = mysql_query($sql);
                while($notebook_row=mysql_fetch_row($notebook_result, MYSQL_ASSOC)){
                        $notebook       = array();
                        $notebook["name"]  = $notebook_row["name"];
                        $notebook["nID"]  = $notebook_row["nID"];
                        $notebook["lastmodifiedtime"]     = $notebook_row["lastmodifiedtime"];
                        $nID    = $notebook_row["ID"]; 

                        $sections        = array();
                        $sql = "select ID , name,sID,lastModifiedTime from Section where nID='$nID'";
                        $section_result = mysql_query($sql);
                        while($section_row=mysql_fetch_row($section_result, MYSQL_ASSOC)){
                                $section        = array();
                                $section["name"]        = $section_row["name"];
                                $section["sID"]         = $section_row["sID"];
                                $section["lastModifiedTime"]    = $section_row["lastModifiedTime"];
                                $sid    = $section_row["ID"];

                                $sql    = "select name,pID,lastModifiedTime from Page where sID='$sid'";
                                $page_result    = mysql_query($sql);
                                $pages   = array();
                                while($page_row=mysql_fetch_row($page_result, MYSQL_ASSOC)){
                                        $page   = array();
                                        $page["name"]   = $page_row["name"];
                                        $page["pID"]    = $page_row["pID"];
                                        $page["lastModifiedTime"]       = $page_row["lastModifiedTime"];
                                        array_push($pages, $page);
                                }
                                $section["page"]        = $pages;
                                array_push($sections, $section);
                        }
                        $notebook["section"] = $sections;
                        array_push($output, $notebook);
                }
                echo json_encode($output, JSON_UNESCAPED_UNICODE);
        }

  	function __destruct()
        {
                if($this->MySQLCon)
                        mysql_close($this->MySQLCon);
        }
}
?>

