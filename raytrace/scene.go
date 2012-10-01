package main

import "math"

type Scene struct {
	AmbientLight Color
	Lights       []Light
	Objects      []Object
}

type Light struct {
	Point
	Color
}

type Object interface {
	Material
	Shape
}

// Hit returns the first hit point for the
// given ray in the scene.
func (s Scene) Hit(start, dir Point) (Hit, bool) {
	var obj Object
	dist := math.Inf(1)
	dir = dir.Normalize()

	for i, o := range s.Objects {
		d, hit := o.Hit(start, dir)
		if !hit {
			continue
		}
		if d > 0 && d < dist {
			obj = s.Objects[i]
			dist = d
		}
	}
	return Hit{Start: start, Direction: dir, Distance: dist, Object: obj}, obj != nil
}

type Hit struct {
	// Start and Direction define the ray that hit the object.
	Start, Direction Point
	Distance         float64
	Object
}

func (h Hit) Point() Point {
	return h.Direction.Scale(h.Distance).Plus(h.Start)
}

type Material interface {
	Color(scene Scene, hit Hit, bounces int) Color
}

type Solid struct {
	C     Color
	Shine float64
	Shape
}

func (s Solid) Color(scene Scene, hit Hit, _ int) Color {
	color := Point(s.C).Times(Point(scene.AmbientLight))
	hitPt := hit.Point()

	for _, l := range scene.Lights {
		dir := l.Minus(hitPt).Normalize()
		if _, hit := scene.Hit(dir.Scale(1e-5).Plus(hitPt), dir); hit {
			continue
		}

		n := s.Shape.Normal(hitPt)
		dot := math.Max(0.0, n.Dot(dir))
		r := n.Scale(2 * dot).Minus(dir).Normalize()

		kd := Point(s.C).Scale(dot)
		rev := hit.Direction.Scale(-1)
		pow := math.Pow(math.Max(0.0, r.Dot(rev)), s.Shine)
		ks := Point{1, 1, 1}.Scale(pow)

		color = color.Plus(Point(l.Color).Times(kd.Plus(ks)))
	}

	return Color(color)
}

type Shape interface {
	// Hit returns distance along a ray at
	// which this entity is hit, or the second
	// argument is false if there is not hit.
	Hit(start, dir Point) (float64, bool)

	// Normal returns the normal of the
	// surface at the given point.
	Normal(Point) Point
}

// A Sphere is a shape with a solid color.
type Sphere struct {
	Center Point
	Radius float64
}

func (s Sphere) Hit(start, dir Point) (float64, bool) {
	a := dir.Dot(dir)
	diff := start.Minus(s.Center)
	b := dir.Dot(diff) * 2
	c := diff.Dot(diff) - s.Radius*s.Radius

	det := b*b - 4*a*c
	if det < 0 {
		return math.Inf(1), false
	}

	t0 := (-b + math.Sqrt(det)) / (2 * a)
	t1 := (-b - math.Sqrt(det)) / (2 * a)
	d := math.Min(t0, t1)
	return d, d > 0
}

func (s Sphere) Normal(pt Point) Point {
	return pt.Minus(s.Center).Normalize()
}
