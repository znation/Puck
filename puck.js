// Globals
var context,
    height,
    width,
    scene;

// Types
function Circle() {}
Circle.prototype.draw = function(ctx, color) {
    ctx.fillStyle = color;
    ctx.beginPath();
    ctx.arc(this.position.x, this.position.y, this.radius, 0, Math.PI*2, true); 
    ctx.closePath();
    ctx.fill();
};

function Scene()
{
    this.players = [new Player(0), new Player(1)];
    this.puck = new Puck;
}
Scene.prototype.draw = function(ctx) {
    this.players[0].draw(ctx);
    this.players[1].draw(ctx);
    this.puck.draw(ctx, "rgb(255,255,255)");
};

function Stick()
{
    this.radius = 32;
    this.position = new Coordinates((width/2)-(this.width/2),
            height-(2 * this.height));
}
Stick.prototype = new Circle;

function Player(playerIdx)
{
    var padding = 16;
    this.idx = playerIdx;
    this.width = (width/2) - (padding * 2);
    this.height = height - (padding * 2);
    this.position = new Coordinates(
            playerIdx == 0 ? padding : (width/2) + (padding),
            padding);
    this.color = this.idx == 0 ? "rgb(255,255,0)" : "rgb(255,0,255)";
    this.stick = new Stick;
}
Player.prototype.draw = function(ctx) {
    context.strokeStyle = this.color;
    context.strokeRect(this.position.x,
            this.position.y,
            this.width,
            this.height);

    this.stick.draw(ctx, this.color);
};

function Puck()
{
    this.radius = 20;
    this.position = new Coordinates(
            (width/2),
            (height/2));
}
Puck.prototype = new Circle;

function Coordinates(x, y) {this.x = x; this.y = y;}

// Functions
function assert(cond)
{
    if (!cond)
    {
        console.log("assertion failed");
        debugger;
    }
}

function init()
{
    var mainCanvas = document.getElementById("mainCanvas");
    context = mainCanvas.getContext("2d");

    width = mainCanvas.offsetWidth;
    height = mainCanvas.offsetHeight;
    mainCanvas.width = width;
    mainCanvas.height = height;

    // Initialize scene
    scene = new Scene;

    // Set draw loop timer
    setInterval(draw, 1000 / 60);

    // Set up event handlers
    document.body.addEventListener("mousemove", handleMouseMove);
}

function handleMouseMove(evt)
{
    // Set horizontal position of stick to be centered on the mouse
    var stick = scene.players[0].stick;
    stick.position.x = evt.clientX;
    stick.position.y = evt.clientY;
}

function draw()
{
    //context.clearRect(0, 0, width, height);

    // Fill with black
    context.fillStyle = "rgb(0,0,0)";
    context.fillRect(0, 0, width, height);

    // Draw scene
    scene.draw(context);
}

window.addEventListener("load", init);

