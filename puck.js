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
    this.position = new Vector(this.padding, this.padding);
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
    for (var i=0; i<2 /*players.length*/; i++)
    {
        this.players[i].stick.move();
    }
    this.puck.move();
};

function Stick(player)
{
    this.player = player;
    this.radius = 32;
    this.position = new Vector(player.position.x + (player.width/2),
            player.position.y + (player.height/2));
    this.nextPosition = this.position;
    this.color = player.color;
    this.angle = 0;
}
Stick.prototype = new Circle;
Stick.prototype.updatePosition = function(x, y) {
    this.nextPosition = new Vector(x, y);
};
Stick.prototype.move = function() {
    this.position = this.nextPosition;
};

function Player(playerIdx, scene)
{
    var padding = 16;
    this.idx = playerIdx;
    this.width = (scene.width/2) - (padding * 2);
    this.height = scene.height - (padding * 2);
    this.position = new Vector(
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
    this.position = new Vector(
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
        var normalVector = new Vector(Math.sin(this.angle),
                Math.cos(this.angle));
        var magnitude = normalVector.distance(new Vector(0, 0));//Math.sqrt((normalVector.x ^ 2) + (normalVector.y ^ 2));
        var unitNormalVector = new Vector(normalVector.x / magnitude, normalVector.y / magnitude);
        this.angle = Math.atan2(unitNormalVector.x, -unitNormalVector.y);
        this.angle = normalizeAngle(this.angle);
    }
    else if (next.position.y < scene.position.y ||
        next.position.y > (scene.position.y + scene.height))
    {
        console.log("Collision with Y wall");
        var normalVector = new Vector(Math.sin(this.angle),
                Math.cos(this.angle));
        var magnitude = normalVector.distance(new Vector(0, 0));//Math.sqrt((normalVector.x ^ 2) + (normalVector.y ^ 2));
        var unitNormalVector = new Vector(normalVector.x / magnitude, normalVector.y / magnitude);
        this.angle = Math.atan2(-unitNormalVector.x, unitNormalVector.y);
        this.angle = normalizeAngle(this.angle);
    }

    // Check for stick boundary
    for (var i=0; i<2 /*players.length*/; i++)
    {
        var player = scene.players[i];
        var stick = player.stick;
        var d = this.position.distance(stick.position);//Math.sqrt(((this.position.x - stick.position.x) ^ 2) + ((this.position.y - stick.position.y) ^ 2));
        if (d <= this.radius + stick.radius)
        {
            // Collision detected
            
            
            var v = this.position.subtract(stick.position);
            var va = normalizeAngle(v.toAngle());
            var vb = va; // turn by 90 degrees
            this.angle = normalizeAngle(vb);
        }
    }
};
Puck.prototype.move = function() {
    this.position.x += this.speed * Math.cos(this.angle);
    this.position.y += this.speed * Math.sin(this.angle);    
};
Puck.prototype.peekNextMove = function() {
    var ret = {};
    ret.position = new Vector(this.position.x + (this.speed * Math.cos(this.angle)),
            this.position.y + (this.speed * Math.sin(this.angle)));
    return ret;
};

function Vector(x, y) {this.x = x; this.y = y;}
Vector.prototype.multiply = function(d) {
    return new Vector(this.x * d, this.y * d);
};
Vector.prototype.add = function(v) {
    return new Vector(this.x + v.x, this.y + v.y);
};
Vector.prototype.subtract = function(v) {
    return new Vector(this.x - v.x, this.y - v.y);
};
Vector.prototype.dot = function(v) {
    return (this.x * v.x) + (this.y * v.y);
};
Vector.prototype.normalize = function() {
    var magnitude = this.distance(new Vector(0,0));
    return new Vector(this.x / magnitude, this.y / magnitude);
};
Vector.prototype.distance = function(v) {
    return Math.sqrt(square(v.x-this.x) + square(v.y-this.y));
};
Vector.prototype.toAngle = function() {
    return Math.atan2(this.x, this.y);
}

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

    stick.updatePosition(x, y);
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

function square(x)
{
    return x * x;
}

window.addEventListener("load", init);

