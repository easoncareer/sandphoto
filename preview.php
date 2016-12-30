<?php
include("sandphoto.inc");

$filename = "/home/wwwroot/api/photo/id/sample.jpg";
$temp_path = "/home/wwwroot/api/photo/id/temp";


$target_type = $_GET["t"];
$container_type = $_GET["c"];
$bgcolorid= $_GET["b"];
$cacheFilename = "preview-" . $target_type . "-" . $container_type ."-".$bgcolorid . ".png";
$cachePath = $temp_path . "/" . $cacheFilename;
if (!file_exists($cachePath))
{
	$parser = new PhotoTypeParser();
	$parser->parse('phototype.txt');
	$cw = $parser->get_width($container_type);
	$ch = $parser->get_height($container_type);
	$tw = $parser->get_width($target_type);
	$th = $parser->get_height($target_type);
	$p = new Photo();
	$p->set_container_size($cw, $ch);
	$p->set_target_size($tw, $th);
	$n = $p->put_photo($filename, $bgcolorid, $parser->get_name($target_type).", ".date("Y-m-d"));
	$p->preview_image($cachePath);
        system("pngquant --ext=.png --force " . $cachePath . " >/dev/null 2>/dev/null");
}
header("location: temp/" . $cacheFilename);
exit();


?>
