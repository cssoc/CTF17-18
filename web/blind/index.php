<!DOCTYPE html>
<html lang="en">
	<head>

		<title>Login</title>

	    <!-- Bootstrap core CSS -->
		<link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">
	</head>

	<body>
	
		<header class="blog-header">
		</header>

		<div class="container">
			<div class="page-header">
				<h1>Login</h1>
			</div>
<?php
	if($_SERVER['REQUEST_METHOD'] == 'GET'):
?>
			<form id="login" class="form-signin" action="/index.php" method="POST">
				<h2 class="form-signin-heading">Enter Account Details</h2>
				
				<label for="inputEmail" class="sr-only">Username</label>
				<input name="username" type="text" id="inputEmail" class="form-control" placeholder="Username" required autofocus>
				
				<label for="inputPassword" class="sr-only">Password</label>
				<input name="password" type="password" id="inputPassword" class="form-control" placeholder="Password" required>
				
				<button class="btn btn-lg btn-primary btn-block" type="submit">Login</button>
    		</form>

<?php elseif($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_POST['username']) && isset($_POST['password'])): ?>
	<?php
		$db = new SQLite3('db.db');
		
		$res = $db->query("SELECT oid, name, password FROM users WHERE name='{$_POST['username']}'");
		
		$data = False;
		if($res)
			$data = $res->fetchArray();
		if($data && $data['password'] == $_POST['password']):
		?>

		<div class="alert alert-success">
		You signed in as <?php echo $data['name'];
		?>
		</div>
		<?php else: ?>
		<div class="alert alert-danger">
		Login Failed.
		</div>
		<?php endif; ?>


<?php endif; ?>

		</div> <!-- /container -->
        <footer class="page-footer font-small blue pt-4 mt-4">
                <div class="container-fluid text-center text-md-left">
                <div class="row">
                <a href="/register.php">Register</a>
                </div>
                </div>
        </footer>
	</body>
</html>

