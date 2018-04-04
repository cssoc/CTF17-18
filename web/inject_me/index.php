<!DOCTYPE html>
<html lang="en">
	<head>

		<title>Login</title>

	    <!-- Bootstrap core CSS -->
		<link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">
     <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script> 
	<script>
	function validate(){
		var usr = $('#inputEmail').val();
		var pass = $('#inputPassword').val();
		if(usr.includes("'") || pass.includes("'"))
			alert("INVALID INPUT, PLEASE DON'T INJECT ME");
		else
			$("#login").submit();
	}
	</script>
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
				
				<button class="btn btn-lg btn-primary btn-block" onclick="validate()" type="button">Login</button>
    		</form>

<?php elseif($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_POST['username']) && isset($_POST['password'])): ?>
	<?php
		$db = new SQLite3('db.db');
		
		$res = $db->query("SELECT oid, name FROM users WHERE name='{$_POST['username']}' AND password='{$_POST['password']}'");
		
		$data = False;
		if($res)
			$data = $res->fetchArray();
		if($data):
		?>

		<div class="alert alert-success">
		You signed in as <?php echo $data['name'];
		if($data['name'] == 'admin')
			echo "<p>Here is your flag: CSSOC{S4nI7iS3_Y0UR_InPU72}</p>";
		?>
		</div>
		<?php else: ?>
		<div class="alert alert-danger">
		Login Failed.
		</div>
		<?php endif; ?>


<?php endif; ?>

		</div> <!-- /container -->


	</body>
</html>

