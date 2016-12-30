<!DOCTYPE html>
<html dir="ltr" lang="zh-CN">
	<head>
		<meta charset="UTF-8" />
		<title>证件照片在线生成</title>
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<link rel="stylesheet" href="//cdn.jsdelivr.net/bootstrap/3.3.7/css/bootstrap.min.css">
		<link rel="stylesheet" href="//cdn.jsdelivr.net/bootstrap.fileinput/4.3.6/css/fileinput.min.css">
	</head>
	<body>
		<div class="well center-block" style="max-width:400px">
			<form id="sandphotoform" action="./sandphoto.php" method="POST" enctype="multipart/form-data">
			<strong>第一步</strong>, 选择证件照的尺寸：<br />
			<?php
			include('./sandphoto.inc');
			$parser = new PhotoTypeParser();
			$parser->parse('./phototype.txt');
			$parser->render_select('target_type', 0, 8);
			?>
			<br /><strong>第二步</strong>, 选择相片纸张尺寸:<br />
			<?php
			$parser->render_select('container_type', 8);
			?>
			<br />
			<p><strong>第三步</strong>, 选择边框颜色：</p>
			<div class="btn-group" data-toggle="buttons" id="bgcolorid">
				<label class="btn btn-primary active">
					<input id="bgcolorid" type="radio" name="bgcolorid" value="blue" checked="checked" /> 蓝色
				</label>
				<label class="btn btn-primary">
					<input id="bgcolorid" type="radio" name="bgcolorid" value="white" /> 白色
				</label>
			</div>
			<br /><br />
			<p>生成的照片会是这样：</p>

			<p>
				<img id="previewImg" alt="" style="max-width:100%"/>
				<img id="tempImg" data-src="holder.js/333x500?text=预览生成中..."/>
			</p>

			<p><strong>第四步</strong>, 选择你的证件照片：</p>

			<p><input id='filename' type='file' name='filename' class='file file-loading' data-show-caption='false' data-show-upload='false' data-allowed-file-extensions='["jpeg", "jpg", "png", "tiff"]' data-max-file-size=1024/></p>

			<p><strong>最后一步</strong>, 点击下载，照片就可以去冲印了：</p>

			<p><input class="btn btn-primary btn-lg btn-block" type="submit" value="下载" /></p>

			<p>如果你的照片比较大，上传会花一些时间，请耐心等待。</p>

			</form>
		</div>
		<script type="text/javascript" src="//cdn.jsdelivr.net/jquery/3.1.1/jquery.min.js"></script>
		<script type="text/javascript" src="//cdn.jsdelivr.net/bootstrap.fileinput/4.3.6/js/fileinput.min.js"></script>
		<script type="text/javascript" src="//cdn.jsdelivr.net/bootstrap/3.3.7/js/bootstrap.min.js"></script>
		<script type="text/javascript" src="//cdn.jsdelivr.net/holder/2.9.4/holder.min.js"></script>
		<script type="text/javascript" src="./sandphoto.js"></script>
	</body>
</html>
