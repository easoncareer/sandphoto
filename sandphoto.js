
$(document).ready(function(){
	$("#target_type").change(updatePreview);
	$("#container_type").change(updatePreview);
	$("#bgcolorid :input").change(updatePreview);
	$("#sandphotoform").submit(checkForm);
	updatePreview();
});

function updatePreview()
{
	$target_type = $("#target_type option:selected").val();
	$container_type = $("#container_type option:selected").val();
	$bgcolorid= $("#bgcolorid input:radio:checked").val();
	if ($target_type && $container_type && $bgcolorid) {
		var img = document.getElementById("previewImg");
		if(img.clientHeight > 0) {	
			$("#tempImg").attr("data-src", "holder.js/"+img.clientWidth+"x"+img.clientHeight+"?text=预览生成中...");
			Holder.run({
				images: "#tempImg"
			});
		}
		$("#previewImg").attr("src", "./preview.php?t=" + $target_type + "&c=" + $container_type + "&b=" + $bgcolorid);
		$("#previewImg").hide();
		$("#tempImg").show();
		$("#previewImg").on('load', function(){
  			$('#tempImg').hide();
			$("#previewImg").show();
		});
	}
}

function checkForm()
{
	if ($("#filename").val() == "")
	{
		alert("请选择照片后再试");
		return false;
	}
	if (!$("#filename").val().match(/jpg|jpeg|png|tiff/i))
	{
		alert("只支持jpeg, jpg, png, tiff文件");
		return false;
	}


	return true;
}
