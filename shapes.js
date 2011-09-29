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

function Stick(player)
{
    this.player = player;
    this.radius = 32;
    this.position = new Vector(player.position.x + (player.width/2),
            player.position.y + (player.height/2));
    this.nextPosition = this.position;
    this.color = player.color;
    this.angle = 0;
    this.speed = 0;
    this.mass = 1000;
}
Stick.prototype = new Circle;
Stick.prototype.updatePosition = function(x, y) {
    this.nextPosition = new Vector(x, y);
};
Stick.prototype.move = function() {
    this.speed = this.nextPosition.distance(this.position);
    this.angle = this.position.subtract(this.nextPosition).toAngle();
    this.position = this.nextPosition;
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
    this.mass = 1;
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
            // Algorithm from http://www.vobarian.com/collisions/2dcollisions2.pdf
            
            var n = this.position.subtract(stick.position);
            var un = n.normalize();
            var ut = new Vector(-un.y, un.x);

            var m1 = this.mass;
            var m2 = stick.mass;

            var v1 = new Vector(Math.sin(this.angle), Math.cos(this.angle)).multiply(this.speed);
            var v2 = new Vector(Math.sin(stick.angle), Math.cos(stick.angle)).multiply(stick.speed);

            var v1n = un.dot(v1);
            var v1t = ut.dot(v1);
            var v2n = un.dot(v2);
            var v2t = ut.dot(v2);

            var vP1t = v1t, vP2t = v2t;
            var vP1n = ((v1n * (m1 - m2)) + (2 * (m2 * v2n))) / (m1 + m2);
            var vP2n = ((v2n * (m2 - m1)) + (2 * (m1 * v1n))) / (m1 + m2);

            var VP1n = un.multiply(vP1n);
            var VP1t = ut.multiply(vP1t);
            var VP2n = un.multiply(vP2n);
            var VP2t = ut.multiply(vP2t);

            var VP1 = VP1n.add(VP1t);
            var VP2 = VP2n.add(VP2t);

            this.angle = Math.atan2(VP1.x, VP1.y);
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

