import java. lang. Math. *;
import java.math.BigDecimal;

public class Triangle extends Shape{
	private double Area;
	private double Perimeter;
	public Triangle(double length) {
		super(length);
	}
	public void setLength(double new_len){
		length=new_len;
	}
	public double getArea(){
		Area = Math.sqrt(3)*length*length/4;
		BigDecimal   b   =   new   BigDecimal(Area);  
		Area   =   b.setScale(2,   BigDecimal.ROUND_HALF_UP).doubleValue();  
		return Area;
	}
	public double getPerimeter(){
		Perimeter = length*3;
		BigDecimal   b   =   new   BigDecimal(Perimeter);  
		Perimeter   =   b.setScale(2,   BigDecimal.ROUND_HALF_UP).doubleValue();  
		return Perimeter;
	}
}