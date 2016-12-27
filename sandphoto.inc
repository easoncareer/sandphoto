<?php

//CONST

class Photo
{
	var $container_width;
	var $container_height;
	var $im;
	var $target_width;
	var $target_height;
	const CM_PER_INCH = 2.54;
	const DPI = 300;

	function __construct()
	{
		$this->im = null;
	}

	function set_container_size($width, $height)
	{
		$this->container_width = $this->get_pixel_from_CM($width);
		$this->container_height = $this->get_pixel_from_CM($height);
	}

	function rotate_container()
	{
		$tmp = $this->container_width;
		$this->container_width = $this->container_height;
		$this->container_height = $tmp;
	}

	function create_empty_image($bgcolorid, $w_start, $h_start, $wn, $hn, $GAP, $text)
	{
		$this->im = imagecreatetruecolor($this->container_width, $this->container_height);
		switch($bgcolorid)
		{
			case "white":
				$color = imagecolorallocate($this->im, 255, 255, 255);
				break;
			case "blue":
				$color = imagecolorallocate($this->im, 0, 0, 120);
				break;
			default:
				$color = imagecolorallocate($this->im, 0, 0, 120);
		}
		$black = imagecolorallocate($this->im, 0, 0, 0);
		$white = imagecolorallocate($this->im, 255, 255, 255);
		imagefill($this->im, 0, 0, $white);
		$x1 = 0;
		$y1 = 0;
		$x2 = $this->container_width;
		$y2 = $this->container_height;
		if($w_start-$GAP > 1)
			$x1 = $w_start-$GAP;
		if($h_start-$GAP > 1)
			$y1 = $h_start-$GAP;
		if(($this->target_width+$GAP)*$wn+$w_start < $x2)
			$x2 = ($this->target_width+$GAP)*$wn+$w_start;
		if(($this->target_height+$GAP)*$hn+$h_start < $y2)
			$y2 = ($this->target_height+$GAP)*$hn+$h_start;
		imagefilledrectangle($this->im, $x1, $y1, $x2, $y2, $color);
		$fontfile = "./msyh.ttf";
		$font_size = 15;
		$angle = 0;

		// Get Bounding Box Size
		$text_box = imagettfbbox($font_size, $angle, $fontfile, $text);

		// Get your Text Width and Height
		$text_width = $text_box[2]-$text_box[0];
		$text_height = $text_box[7]-$text_box[1];

		imagettftext($this->im, $font_size, $angle, floor(($this->container_width - $text_width)/2), floor(($this->container_height + $y2 - $text_height)/2), $black, $fontfile, $text);
	}

	function get_pixel_from_CM($value)
	{
		$result = $value * self::DPI / self::CM_PER_INCH;
		return floor($result);
	}

	function set_target_size($w, $h)
	{
		$this->target_width = $this->get_pixel_from_CM($w);
		$this->target_height = $this->get_pixel_from_CM($h);
	}

	function put_photo($path, $bgcolorid="blue", $text="By iYUYUE.net")
	{
		$size = getimagesize($path);
		$w = $size[0];
		$h = $size[1];

		// cut the source photo
		$ratio1 = $w / $this->target_width;
		$ratio2 = $h / $this->target_height;
		if ($ratio1 > $ratio2)
		{
			$cut_w = ($this->target_width * $ratio2);
			$cut_h = $h;
			$cut_x = ($w - $cut_w) / 2;
			$cut_y = 0;
		}
		else
		{
			$cut_h = ($this->target_height * $ratio1);
			$cut_w = $w;
			$cut_y = ($h - $cut_h) / 2;
			$cut_x = 0;
		}

		$GAP = 3;

		$wn = floor($this->container_width / ($this->target_width + $GAP));
		$hn = floor($this->container_height/ ($this->target_height + $GAP));

		$wn2 = floor($this->container_height/ ($this->target_width + $GAP));
		$hn2 = floor($this->container_width/ ($this->target_height + $GAP));

		if ($wn2 * $hn2 > $wn * $hn)
		{
			$this->rotate_container();
			$wn = $wn2;
			$hn = $hn2;
		}

		$w_start = ($this->container_width- $wn * ($this->target_width + $GAP) + $GAP) / 2;
		$h_start = ($this->container_height- $hn * ($this->target_height + $GAP) + $GAP) / 2;
		$this->create_empty_image($bgcolorid, $w_start, $h_start, $wn, $hn, $GAP, $text);
		$im2 = $this->imagecreatefromany($path);
		for ($i = 0; $i < $wn; $i++)
		{
			$pos_x = $w_start + ($this->target_width + $GAP) * $i;
			for ($j = 0; $j < $hn; $j++)
			{
				$pos_y = $h_start + ($this->target_height + $GAP) * $j;
				imagecopyresampled($this->im, $im2, $pos_x, $pos_y, $cut_x, $cut_y, $this->target_width, $this->target_height, $cut_w, $cut_h);
			}
		}
		imagedestroy($im2);
		return $wn * $hn;
	}

	function render_image()
	{
		header("Content-Type: image/jpeg");
		imagejpeg($this->im, NULL, 100);
	}

	function download_image($filename)
	{
		$ua = $_SERVER["HTTP_USER_AGENT"];
		$encoded_filename =  urlencode($filename);
		header("Content-Type: application/octet-stream");
		if (preg_match("/MSIE/", $ua))
		{
			header("Content-Disposition: attachment; filename=\"" . $encoded_filename ."\"");
		}
		else 
		{
			header("Content-Disposition: attachment; filename=\"" . $filename ."\"");
		}
		imagejpeg($this->im, NULL, 100);
	}

	function preview_image($filename)
	{
		$width = 500;
		$height = 500;
		$ratio1 = $this->container_width / $width;
		$ratio2 = $this->container_height / $height;
		$ratio =  ($ratio1 > $ratio2) ? $ratio1 : $ratio2;
		$w = floor($this->container_width / $ratio);
		$h = floor($this->container_height / $ratio);
		//print $w . ":" . $h;
		$im = imagecreatetruecolor($w, $h);
		imagecopyresampled($im, $this->im, 0, 0, 0, 0, $w, $h, $this->container_width, $this->container_height);
		header("Content-Type: image/png");
		imagepng($im, $filename);
		imagedestroy($im);
	}

	function __destruct()
	{
		if ($this->im != null)
		{
			imagedestroy($this->im);
		}
	}

	function imagecreatefromany($filepath)
	{ 
		$type = exif_imagetype($filepath); // [] if you don't have exif you could use getImageSize() 
		$allowedTypes = array( 
				2,  // [] jpg 
				3,  // [] png 
				6   // [] bmp 
				); 
		if (!in_array($type, $allowedTypes)) { 
			return false; 
		} 
		switch ($type) { 
			case 2 : 
				$im = imagecreatefromjpeg($filepath); 
				break; 
			case 3 : 
				$im = imagecreatefrompng($filepath); 
				break; 
			case 6 : 
				$im = imagecreatefrombmp($filepath); 
				break; 
		}    
		return $im;  
	} 
};

class PhotoTypeParser
{
	var $count;
	var $name_array;
	var $width_array;
	var $height_array;
	function parse($path)
	{
		$fh = fopen($path, "r");
		$this->count = 0;
		while (($line= fgets($fh)) !== FALSE)
		{
			$data = explode(',', $line);
			if (count($data) != 3) continue;

			$this->name_array[$this->count] = $data[0];
			$this->width_array[$this->count] = $data[1];
			$this->height_array[$this->count] = $data[2];
			$this->count ++;
		}
		fclose($fh);
	}

	function get_name($i)
	{
		return $this->name_array[$i];
	}
	function get_width($i)
	{
		return $this->width_array[$i];
	}
	function get_height($i)
	{
		return $this->height_array[$i];
	}

	function render_select($name, $min_size = 0, $max_size=100000)
	{
		print("<select name='$name' id='$name' class='form-control'>");
		for ($i = 0; $i < $this->count; $i++)
		{
			if ($this->width_array[$i] > $min_size && $this->width_array[$i] < $max_size)
			{
				printf("<option value='%d'>%s (%.2fcm * %.2fcm)</option>",
						$i,
						$this->name_array[$i],
						$this->width_array[$i],
						$this->height_array[$i]);
			}
		}
		print("</select>");
	}
};
?>
