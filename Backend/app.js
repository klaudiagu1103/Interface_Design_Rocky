var createError = require("http-errors");
var express = require("express");
var path = require("path");
var cookieParser = require("cookie-parser");
var logger = require("morgan");

// Play spotify script setup
const shell = require("shelljs");
const pathToPlaySpotify = "osascript play_spotify.scpt";
const pathToStopSpotify = "osascript stop_spotify.scpt";

var indexRouter = require("./routes/index");

var app = express();

// view engine setup
app.set("views", path.join(__dirname, "views"));
app.set("view engine", "jade");

app.use(logger("dev"));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, "public")));

app.use("/", indexRouter);

// Route addressed by arduino to play song
app.get("/playRocky", function(req, res) {
  console.log("Attempting to play Rocky");

  // Execute apple script "play_spotify" which triggers Spotify
  if (shell.exec(pathToPlaySpotify).code == 0) {
    console.log("Playing rocky");
    res.send("ADRIAN!");
  } else {
    shell.exit(1);
    res.send("An error occured playing the script.");
  }
});
// Route addressed by arduino to play song
app.get("/stopRocky", function(req, res) {
  console.log("Attempting to play Rocky");

  // Execute apple script "play_spotify" which triggers Spotify
  if (shell.exec(pathToStopSpotify).code == 0) {
    console.log("Stopping rocky");
    res.send("ADRIAN!");
  } else {
    shell.exit(1);
    res.send("An error occured playing the script.");
  }
});

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get("env") === "development" ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render("error");
});

module.exports = app;
