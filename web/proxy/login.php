<!doctype html>
<html lang="en">
	<head>
		<title>Login</title>
		<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">

	</head>
	<body>
	<div class="container">
		<div class="jumbotron">
			<h1>Login</h1>
			<p>Admin login page. If you're not an admin: pls go<p>
		</div>
	<form method="post">
		<div class="form-group">
		<label for="id">Gibe password</label>
		<input type="password" id="password" class="form-control" name="password" placeholder="Password">
		</div>
		<button type="submit" class="btn btn-primary">View</button>

	</form>
	
	<?php if(isset($_POST['password'])){
		$memcached = new Memcached();
		$memcached->addServer('localhost', 11211);
		if($_POST['password'] == $memcached->get('password'))
			echo $memcached->get('flag');
		else
			echo "WRONG PASSWORD";
	}
	?>
	</div>
	</body>
</html>
