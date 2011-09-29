// Globals
var context,
    height,
    width,
    scene;

// Types
function Circle() {}
Circle.prototype.draw = function(ctx) {
    assert(this.position != null);
    assert(this.color != null);
    assert(this.radius != null);

    ctx.fillStyle = this.color;
    ctx.beginPath();
    ctx.arc(this.position.x, this.position.y, this.radius, 0, Math.PI*2, true); 
    ctx.closePath();
    ctx.fill();
};

function StrokeRectangle() {}
StrokeRectangle.prototype.draw = function(ctx) {
    assert(this.color != null);
    assert(this.position != null);
    assert(this.width != null);
    assert(this.height != null);

    ctx.strokeStyle = this.color;
    ctx.strokeRect(this.position.x,
            this.position.y,
            this.width,
            this.height);
};

function Scene()
{
    this.color = "rgb(0,255,255)";
    this.padding = 16;
    this.width = width - (2 * this.padding);
    this.height = height - (2 * this.padding);
    this.position = new Coordinates(this.padding, this.padding);
    this.puck = new Puck;
    this.players = [new Player(0, this), new Player(1, this)];
}
Scene.prototype = new StrokeRectangle;
Scene.prototype.detectCollisions = function() {
    this.puck.detectCollisions();
};
Scene.prototype.draw = function(ctx) {
    this.constructor.prototype.draw.call(this, ctx);
    this.players[0].draw(ctx);
    this.players[1].draw(ctx);
    this.puck.draw(ctx);
};
Scene.prototype.move = function() {
    this.puck.move();
};

function Stick(player)
{
    this.player = player;
    this.radius = 32;
    this.position = new Coordinates(player.position.x + (player.width/2),
            player.position.y + (player.height/2));
    this.color = player.color;
}
Stick.prototype = new Circle;

function Player(playerIdx, scene)
{
    var padding = 16;
    this.idx = playerIdx;
    this.width = (scene.width/2) - (padding * 2);
    this.height = scene.height - (padding * 2);
    this.position = new Coordinates(
            playerIdx == 0 ? scene.padding + padding : (scene.width/2) + scene.padding + padding,
            scene.padding + padding);
    this.color = this.idx == 0 ? "rgb(255,255,0)" : "rgb(255,0,255)";
    this.stick = new Stick(this);
}
Player.prototype = new StrokeRectangle;
Player.prototype.draw = function(ctx) {
    this.constructor.prototype.draw.call(this, ctx);
    this.stick.draw(ctx, this.color);
};

function Puck()
{
    this.radius = 20;
    this.position = new Coordinates(
            (width/2),
            (height/2));
    this.angle = Math.random() * Math.PI * 2;
    this.speed = 5;
    this.color = "rgb(0,255,255)";
}
Puck.prototype = new Circle;
Puck.prototype.detectCollisions = function() {
    // Puck is moving fast, so check for collisions with puckNext.
    var next = this.peekNextMove();

    // Check for scene boundary
    if (next.position.x < scene.position.x ||
        next.position.x > (scene.position.x + scene.width))
    {
        console.log("Collision with X wall");
        var normalVector = new Coordinates(Math.sin(this.angle),
                Math.cos(this.angle));
        var magnitude = Math.sqrt((normalVector.x ^ 2) + (normalVector.y ^ 2));
        var unitNormalVector = new Coordinates(normalVector.x / magnitude, normalVector.y / magnitude);
        this.angle = Math.atan2(unitNormalVector.x, -unitNormalVector.y);
        this.angle = normalizeAngle(this.angle);
    }
    else if (next.position.y < scene.position.y ||
        next.position.y > (scene.position.y + scene.height))
    {
        console.log("Collision with Y wall");
        var normalVector = new Coordinates(Math.sin(this.angle),
                Math.cos(this.angle));
        var magnitude = Math.sqrt((normalVector.x ^ 2) + (normalVector.y ^ 2));
        var unitNormalVector = new Coordinates(normalVector.x / magnitude, normalVector.y / magnitude);
        assert(Math.asin(normalVector.x) + Math.acos(normalVector.y) == this.angle);
        this.angle = Math.atan2(-unitNormalVector.x, unitNormalVector.y);
        this.angle = normalizeAngle(this.angle);
    }

    // Check for stick boundary
    for (var i=0; i<2 /*players.length*/; i++)
    {
        var player = scene.players[i];
        var stick = player.stick;
        var distance = Math.sqrt(((this.position.x - stick.position.x) ^ 2) + ((this.position.y - stick.position.y) ^ 2));
        if (distance <= this.radius + stick.radius)
        {
            // Collision!
            //
        }
    }
};
Puck.prototype.move = function() {
    this.position.x += this.speed * Math.cos(this.angle);
    this.position.y += this.speed * Math.sin(this.angle);    
};
Puck.prototype.peekNextMove = function() {
    var ret = {};
    ret.position = new Coordinates(this.position.x + (this.speed * Math.cos(this.angle)),
            this.position.y + (this.speed * Math.sin(this.angle)));
    return ret;
};

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
    // Set position of stick to be centered on the mouse
    var player = scene.players[0];
    var stick = player.stick;

    // Limit coordinates of stick to within the player's boundaries
    var x = evt.clientX;
    var y = evt.clientY;

    if (x > (player.position.x + player.width))
    {
        x = player.position.x + player.width;
    }
    else if (x < (player.position.x))
    {
        x = player.position.x;
    }
    if (y > player.position.y + player.height)
    {
        y = player.position.y + player.height;
    }
    else if (y < player.position.y)
    {
        y = player.position.y;
    }

    stick.position.x = x;
    stick.position.y = y;
}

function draw()
{
    //context.clearRect(0, 0, width, height);

    // Fill with black
    context.fillStyle = "rgb(0,0,0)";
    context.fillRect(0, 0, width, height);

    // Detect collisions
    scene.detectCollisions();

    // Update positions
    scene.move();

    // Draw scene
    scene.draw(context);
}

function normalizeAngle(angle)
{
    var ret = angle;
    while (ret > (Math.PI * 2))
    {
        ret -= (Math.PI * 2);
    }
    while (ret < 0)
    {
        ret += (Math.PI * 2);
    }
    return ret;
}

window.addEventListener("load", init);

