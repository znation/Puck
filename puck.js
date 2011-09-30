// Globals
var context,
    height,
    width,
    scene, // Game model scene
    world, // Box2D model world
    gameCircles, boxCircles;

// Types
function Scene()
{
    this.color = "rgb(0,255,255)";
    this.padding = 16;
    this.width = width - (2 * this.padding);
    this.height = height - (2 * this.padding);
    this.position = new Vector(this.padding, this.padding);
    this.puck = new Puck;
    this.players = [new Player(0, this), new Player(1, this)];

    // Construct a rectangle in the world for each border
	var boxSd = new b2BoxDef();
	boxSd.extents.Set(this.width / 200.0, this.height / 200.0);
	var boxBd = new b2BodyDef();
	boxBd.AddShape(boxSd);
	boxBd.position.Set(this.padding / 100.0, this.padding / 100.0);
	world.CreateBody(boxBd);
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

    // Initialize Box2D world
    var worldAABB = new b2AABB;
    worldAABB.minVertex.Set(0, 0);
    worldAABB.maxVertex.Set(width / 100.0, height / 100.0);
    var gravity = new b2Vec2(0, 0);
    var doSleep = false;
    world = new b2World(worldAABB, gravity, doSleep);

    // Initialize scene
    scene = new Scene;

    // Populate Box2D world
    populateWorld();

    // Set draw loop timer
    setInterval(draw, 1000 / 60);

    // Set up event handlers
    document.body.addEventListener("mousemove", handleMouseMove);
}

function populateWorld()
{
    gameCircles = [
        scene.puck,
        scene.players[0].stick,
        scene.players[1].stick
    ];
    boxCircles = [];

    for (var i=0; i<gameCircles.length; i++)
    {
        var circle = gameCircles[i];
        var circleSd = new b2CircleDef;
        circleSd.density = 1.0;
        circleSd.radius = circle.radius / 100.0;
        circleSd.restitution = 1.0;
        circleSd.friction = 0;
        var circleBd = new b2BodyDef;
        circleBd.AddShape(circleSd);
        circleBd.position.Set(circle.position.x / 100.0, circle.position.y / 100.0);
        var circleBody = world.CreateBody(circleBd);
        boxCircles.push(circleBody);
    }
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
    world.Step(1.0/60, 1);

    context.clearRect(0, 0, width, height);

    // Fill with black
    context.fillStyle = "rgb(0,0,0)";
    context.fillRect(0, 0, width, height);

    // Detect collisions
    //scene.detectCollisions();

    // Update positions
    for (var i=0; i<gameCircles.length; i++)
    {
        var circle = gameCircles[i];
        var circleBody = boxCircles[i];
        circle.position.x = circleBody.m_position.x * 100.0;
        circle.position.y = circleBody.m_position.y * 100.0;
    }

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

