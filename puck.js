// Globals
var scalingFactor = 200.0,
    context,
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
    this.position = new b2Vec2(this.padding, this.padding);
    this.puck = new Puck;
    this.players = [new Player(0, this), new Player(1, this)];

    // Construct a border box to keep everything inside
    var edges = [
        { // bottom
            width: width,
            height: this.padding,
            x: 0,
            y: height
        },
        { // top
            width: width,
            height: this.padding,
            x: 0,
            y: 0
        },
        { // left
            width: this.padding,
            height: height,
            x: 0,
            y: 0
        },
        { // right
            width: this.padding,
            height: height,
            x: width,
            y: 0
        }
    ];

    for (var i=0; i<edges.length; i++)
    {
        var edge = edges[i];
        edge.width = pixelToBoxCoords(edge.width);
        edge.height = pixelToBoxCoords(edge.height);
        edge.x = pixelToBoxCoords(edge.x);
        edge.y = pixelToBoxCoords(edge.y);

        var boxSd = new b2BoxDef();
        boxSd.extents.Set(edge.width, edge.height);
        boxSd.density = 0;
        boxSd.restitution = 1.0;
        boxSd.friction = 0;
        var boxBd = new b2BodyDef();
        boxBd.AddShape(boxSd);
        boxBd.position.Set(edge.x, edge.y);
        var body = world.CreateBody(boxBd);
        body.m_angularDamping = 0;
        body.m_linearDamping = 0;
    }
}
Scene.prototype = new StrokeRectangle;
Scene.prototype.draw = function(ctx) {
    this.constructor.prototype.draw.call(this, ctx);
    this.players[0].draw(ctx);
    this.players[1].draw(ctx);
    this.puck.draw(ctx);
};
Scene.prototype.move = function() {
    this.players[0].stick.move();
};

function Player(playerIdx, scene)
{
    var padding = 16;
    this.idx = playerIdx;
    this.width = (scene.width/2) - (padding * 1.5);
    this.height = scene.height - (padding * 2);
    this.position = new b2Vec2(
            playerIdx == 0 ? scene.padding + padding : (scene.width/2) + scene.padding + (0.5 * padding),
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
        //console.log("assertion failed");
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
    worldAABB.maxVertex.Set(pixelToBoxCoords(width), pixelToBoxCoords(height));
    var gravity = new b2Vec2(0, 0);
    var doSleep = true;
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
        circleSd.radius = pixelToBoxCoords(circle.radius);
        circleSd.restitution = 1.0;
        circleSd.friction = 0;
        var circleBd = new b2BodyDef;
        circleBd.AddShape(circleSd);
        circleBd.position.Set(pixelToBoxCoords(circle.position.x), pixelToBoxCoords(circle.position.y));
        

        var circleBody = world.CreateBody(circleBd);
        boxCircles.push(circleBody);

        if (circle != scene.puck)
        {
            var mousedef/*:b2MouseJointDef*/ = new b2MouseJointDef();
            mousedef.body1 = world.GetGroundBody();
            mousedef.body2 = circleBody;
            mousedef.target.Set(circleBd.position.x, circleBd.position.y);
            mousedef.collideConnected = true;
            mousedef.maxForce = 30000 * circleBody.GetMass();
            mousejoint = world.CreateJoint(mousedef) /*as b2MouseJoint*/;
            circle.boxMouseJoint = mousejoint;
        }
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
    // Update mouse-controlled positions
    scene.move();

    // Update physics (step world)
    world.Step(1.0/60, 1);

    // Fill with black
    context.fillStyle = "rgb(0,0,0)";
    context.fillRect(0, 0, width, height);

    // Update game positions based on physics
    for (var i=0; i<gameCircles.length; i++)
    {
        var circle = gameCircles[i];
        var circleBody = boxCircles[i];
        circle.position.x = boxToPixelCoords(circleBody.m_position.x);
        circle.position.y = boxToPixelCoords(circleBody.m_position.y);
    }

    // Draw scene
    scene.draw(context);
}

function boxToPixelCoords(x)
{
    /*
    assert(x >= 0.1);
    assert(x <= 10.0);
    */
    var ret = x * scalingFactor;
    return ret;
}

function pixelToBoxCoords(x)
{
    var ret = x / scalingFactor;
    /*
    assert(ret >= 0.1);
    assert(ret <= 10.0);
    */
    return ret;
}

window.addEventListener("load", init);

