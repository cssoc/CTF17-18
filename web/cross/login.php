<?php
	session_start();
	if(in_array("id", $_SESSION)){
		header('Location: /');
		exit();

	}
?>

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
			<form class="form-signin" action="/login.php" method="POST">
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
		
		$stmt = $db->prepare('SELECT oid, name, admin, password FROM users WHERE name=:name');
		$stmt->bindParam(':name', $_POST['username'], SQLITE3_TEXT);
		
		$res = $stmt->execute();


		$data = False;
		if($res)
			$data = $res->fetchArray();
		if($data && password_verify($_POST['password'], $data['password'])):
			$_SESSION['id'] = $data['rowid'];
			$_SESSION['admin'] = $data['admin'];
			$_SESSION['name'] = $data['name'];
		?>

		<div class="alert alert-success">
		You signed in as <?php echo $data['name'] ?>
		</div>
		<?php else: ?>
		<div class="alert alert-danger">
		Login Failed.
		</div>
		<?php endif; ?>


<?php endif; ?>
        <footer class="page-footer font-small blue pt-4 mt-4">
                <div class="container-fluid text-center text-md-left">
                <?php if(isset($_SESSION['id'])): ?>
                <div class="row">
                <a href="logout.php">Logout</a>
                </div>
                <?php else: ?>
                <div class="row">
                <a href="login.php">Login</a>
                </div>
                <div class="row">
                <a href="register.php">Register</a>
                </div>
                <?php endif; ?>
                <div class="row">
                <a href="/">Index</a>
                </div>
                </div>
        </footer>


		</div> <!-- /container -->


	</body>
</html>

