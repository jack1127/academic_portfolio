import java. lang. Math. *;
import java.math.BigDecimal;

public class Circle extends Shape{
	private double Area;
	private double Perimeter;
	public Circle(double length) {
		super(length);
	}
	public void setLength(double new_len){
		length=new_len;
		return;
	}
	public double getArea(){
		Area = Math.PI*(length/2)*(length/2);
		BigDecimal   b   =   new   BigDecimal(Area);  
		Area   =   b.setScale(2,   BigDecimal.ROUND_HALF_UP).doubleValue();  
		return Area;
	}
	public double getPerimeter(){
		Perimeter = Math.PI*length;
		BigDecimal   b   =   new   BigDecimal(Perimeter);  
		Perimeter   =   b.setScale(2,   BigDecimal.ROUND_HALF_UP).doubleValue();  
		return Perimeter;
	}
}