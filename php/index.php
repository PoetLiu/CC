<?php
#echo $_SERVER["PHP_SELF"];
$paramArray	= explode("/", $_SERVER["PHP_SELF"]);
$getBindWidthInfo360 = array(
	"Bandwidth_Id"		=> "1",
	"Bandwidth_valid"	=> "0,20",
	"Bandwidth_max"		=> "0,30",
	"Bandwidth_type"	=> "10003",
	"Bandwidth_province"	=> "sc",
	"min_tryout"		=> "300",
	"success_flag"		=> "0",
	"client_ip"		=> "192.168.7.101",
	"servicetype"		=> "1001",
	"province_name"		=> "\u56db\u5ddd\u7701",
	"city_name"		=> "\u6210\u90fd\u5e02",
	"district_name"		=> "\u9ad8\u65b0\u533a",
	"isp_name"		=> "\u4e2d\u56fd\u7535\u4fe1"
);
$speedUp	= array(
	"begin_t"		=> "0",
	"end_t"			=> "0"
);
$speedEnd	= array(
	"begin_t"		=> "0",
	"end_t"			=> "0"
);
$memberpanel = array(
	"getBindWidthInfo360"	=> $getBindWidthInfo360, 
	"speedUp"		=> $speedUp, 
	"speedEnd"		=> $speedEnd
);

$login	= array(
	"spdstatus"		=> "",
	"vipexpretime"		=> "",
	"vipexpredate"		=> false
);
$S360 = array(
	"login"			=> $login
);

$modArray	= array(
	"memberpanel"		=> $memberpanel, 
	"S360"			=> $S360
);

if (!array_key_exists($paramArray[2], $modArray)) {
	echo $paramArray[2] . " not exists.";
	exit;
}
if (!array_key_exists($paramArray[3], $modArray[$paramArray[2]])) {
	echo $paramArray[3] . " not exists " . "in mod " . $paramArray[2];
	exit;
}

$res;
if (!strcmp($paramArray[2], "memberpanel")) {
	$res	= array(
		"id"		=> 1,
		"msg"		=> $modArray[$paramArray[2]][$paramArray[3]],
	);
} else {
	$res	= array(
		"code"		=> 1,
		"msg"		=> "",
	);
	$res	= array_merge($res, $modArray[$paramArray[2]][$paramArray[3]]);
}
echo json_encode($res);
?>
