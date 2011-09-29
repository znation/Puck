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
    function square(x)
    {
        return x * x;
    }
    return Math.sqrt(square(v.x-this.x) + square(v.y-this.y));
};
Vector.prototype.toAngle = function() {
    return Math.atan2(this.x, this.y);
}

