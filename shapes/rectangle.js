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

