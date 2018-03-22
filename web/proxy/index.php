<?php
$BAD = array("127.0.0.1", "localhost");
$FILES = array(".jpg", ".png", ".gif");

function filter_url($url)
{
	global $BAD, $FILES;

	$arr = parse_url($url);
	$end = substr($arr['path'], -4);

	if($arr['scheme'] == "file")
		return False;
	if(in_array($arr['host'], $BAD))
		return False;
	if(!in_array($end, $FILES))
		return False;
	return True;
}
?>

<!doctype html>
<html lang="en">
	<head>
		<title>Image Proxy Viewer Service</title>
		<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">

	</head>
	<body>
	<div class="container">
		<div class="jumbotron">
			<h1>Image Proxy Viewer Service</h1>
			<p>A proxy a day keeps the feds away.<p>
		</div>
	<form action="/" method="get">
		<div class="form-group">
		<label for="id">URL must be jpg, gif or png.</label>
		<input type="text" id="url" class="form-control" name="url" placeholder="URL of image you want to view">
		</div>
		<button type="submit" class="btn btn-primary">View</button>

	</form>
	
	<?php if(isset($_GET['url']) && filter_url($_GET['url'])){
	$curl = curl_init();

	curl_setopt($curl, CURLOPT_URL, $_GET['url']);
	curl_setopt($curl, CURLOPT_TIMEOUT, 5);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

	$result = curl_exec($curl);
	$result = base64_encode($result);

	curl_close($curl);
	?>
	<img src="data:image/<?php substr($_GET['url'], -3)?>;base64,<?php
	echo $result; ?>
	" />
	<?php
	} ?>

	<footer class="page-footer font-small blue pt-4 mt-4">
		<div class="container-fluid text-center text-md-left">
		<div class="row">
		<a href="login.php">Login</a>
		</div>
		<div class="row">
		<a href="login_src.php">Login Source</a>
		</div>
		<div class="row">
		<a href="index_src.php">Index Source</a>
		</div>
		</div>
	</footer>

	</div>
	</body>
</html>
