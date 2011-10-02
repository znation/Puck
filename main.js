// Globals
var scalingFactor = 200.0,
    frameRate = 30.0,
    context,
    height,
    width,
    scene, // Game model scene
    world, // Box2D model world
    gameCircles, boxCircles;

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
    setInterval(draw, 1000 / frameRate);

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
        circleSd.density = circle == scene.puck ? 1 : 0.1;
        circleSd.radius = pixelToBoxCoords(circle.radius);
        circleSd.restitution = 1.0;
        circleSd.friction = 0;
        var circleBd = new b2BodyDef;
        circleBd.AddShape(circleSd);
        circleBd.position.Set(pixelToBoxCoords(circle.position.x), pixelToBoxCoords(circle.position.y));
        

        var circleBody = world.CreateBody(circleBd);
        boxCircles.push(circleBody);
        circle.boxCircle = circleBody;

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
    // Apply constraints
    scene.puck.applyConstraints();
    
    // Update mouse-controlled positions
    scene.move();

    // Update physics (step world)
    world.Step(1.0/frameRate, 1);

    // Check contact list for ball-wall collisions
    scene.puck.detectCollisions();

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

