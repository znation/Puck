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

function Rectangle() {}
Rectangle.prototype.draw = function(ctx) {
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

