// Types
function Circle() {}
Circle.prototype.draw = function(ctx) {
    /*
    assert(this.position != null);
    assert(this.color != null);
    assert(this.radius != null);
    */

    ctx.fillStyle = this.color;
    ctx.beginPath();
    ctx.arc(this.position.x, this.position.y, this.radius, 0, Math.PI*2, true); 
    ctx.closePath();
    ctx.fill();
};

function StrokeRectangle() {}
StrokeRectangle.prototype.draw = function(ctx) {
    /*
    assert(this.color != null);
    assert(this.position != null);
    assert(this.width != null);
    assert(this.height != null);
    */

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
    this.position = new b2Vec2(player.position.x + (player.width/2),
            player.position.y + (player.height/2));
    this.nextPosition = this.position;
    this.color = player.color;
    this.angle = 0;
    this.speed = 0;
}
Stick.prototype = new Circle;
Stick.prototype.updatePosition = function(x, y) {
    this.nextPosition = new b2Vec2(x, y);
};
Stick.prototype.move = function() {
    this.position = this.nextPosition;
    this.boxMouseJoint.SetTarget(new b2Vec2(pixelToBoxCoords(this.position.x), pixelToBoxCoords(this.position.y)));
};

function Puck()
{
    this.radius = 20;
    this.position = new b2Vec2(
            (width/2),
            (height/2));
    this.angle = Math.random() * Math.PI * 2;
    this.speed = 5;
    this.color = "rgb(0,255,255)";
}
Puck.prototype = new Circle;

