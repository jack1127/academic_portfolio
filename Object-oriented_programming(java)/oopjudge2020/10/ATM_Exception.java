public class ATM_Exception extends Exception {
	public enum ExceptionTYPE {AMOUNT_INVALID, BALANCE_NOT_ENOUGH;}
	public ATM_Exception(ExceptionTYPE ex_type) {
		super(ex_type.toString());
	}
}