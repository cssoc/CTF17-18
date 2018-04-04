<!DOCTYPE html>
<html lang="en">
	<head>

		<title>Register</title>

	    <!-- Bootstrap core CSS -->
		<link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">
		
	</head>

	<body>
	
		<header class="blog-header">
		</header>

		<div class="container">
			<div class="page-header">
				<h1>Register an Account</h1>
			</div>
<?php
	if($_SERVER['REQUEST_METHOD'] == 'GET'):
?>
			<form class="form-signin" action="/register.php" method="POST">
				<h2 class="form-signin-heading">Enter Account Details</h2>
				
				<label for="inputEmail" class="sr-only">Username</label>
				<input name="username" type="text" id="inputEmail" class="form-control" placeholder="Username" required autofocus>
				
				<label for="inputPassword" class="sr-only">Password</label>
				<input name="password" type="password" id="inputPassword" class="form-control" placeholder="Password" required>
				
				<button class="btn btn-lg btn-primary btn-block" type="submit">Register</button>
    		</form>

<?php elseif($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_POST['username']) && isset($_POST['password'])): ?>
	<?php
		$db = new SQLite3('db.db');
		
		$stmt = $db->prepare('INSERT INTO users(name, password) VALUES (:name, :password)');
		$stmt->bindParam(':name', $_POST['username'], SQLITE3_TEXT);
		$stmt->bindParam(':password', $_POST['password'], SQLITE3_TEXT);
		
		$res = $stmt->execute();

		if($res): ?>

		<div class="alert alert-success">
		You signed up
		</div>
		<?php else: ?>
		<div class="alert alert-danger">
		Sign up failed, maybe someone is already using that name?
		</div>
		<?php endif; ?>


<?php endif; ?>
        <footer class="page-footer font-small blue pt-4 mt-4">
                <div class="container-fluid text-center text-md-left">
                <div class="row">
                <a href="/">Index</a>
                </div>
                </div>
        </footer>


		</div> <!-- /container -->


	</body>
</html>

