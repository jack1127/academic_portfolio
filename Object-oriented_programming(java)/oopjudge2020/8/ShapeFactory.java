public class ShapeFactory {
	public enum Type { Circle, Square, Triangle;}
	public Shape createShape(ShapeFactory.Type shapeType, double length) {
		if(shapeType.name()=="Circle") {
			return new Circle(length);
		}
		else if(shapeType.name()=="Square") {
			return new Square(length);
		}
		else if(shapeType.name()=="Triangle") {
			return new Triangle(length);
		}
		return null;
	}
}