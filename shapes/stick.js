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
Stick.prototype.draw = function(ctx) {
    var innerCircleWidth = Math.floor(this.radius / 10.0);
    var outerCircleWidth = Math.floor(this.radius / 15.0);
    innerCircleWidth = Math.max(1, innerCircleWidth);
    outerCircleWidth = Math.max(innerCircleWidth+1, outerCircleWidth);
    var innerCircleRadius = this.radius / 2;

    ctx.strokeStyle = this.color;
    ctx.lineWidth = outerCircleWidth;
    ctx.beginPath();
    ctx.arc(this.position.x, this.position.y, this.radius, 0, Math.PI*2, true); 
    ctx.closePath();
    ctx.stroke();

    ctx.strokeStyle = this.color;
    ctx.lineWidth = innerCircleWidth;
    ctx.beginPath();
    ctx.arc(this.position.x, this.position.y, innerCircleRadius, 0, Math.PI*2, true); 
    ctx.closePath();
    ctx.stroke();
};

