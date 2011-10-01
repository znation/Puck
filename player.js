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
Player.prototype = new Rectangle;
Player.prototype.draw = function(ctx) {
    this.constructor.prototype.draw.call(this, ctx);
    this.stick.draw(ctx, this.color);
};

