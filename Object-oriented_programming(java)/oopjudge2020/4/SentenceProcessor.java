public class SentenceProcessor {
	/*public static void main(String argv[]){
		SentenceProcessor sp = new SentenceProcessor();
		System.out.println(sp.removeDuplicatedWords("Hello Hello World I love love the World I lovelove the World"));
		System.out.println(sp.removeDuplicatedWords("Buffalo buffalo Buffalo buffalo buffalo buffalo Buffalo buffalo"));
		System.out.println(sp.removeDuplicatedWords("a a la a la carte A la La carte Carte A a la la"));
		System.out.println(sp.replaceWord("major", "minor", "The major problem is how to sing in A major"));
		System.out.println(sp.replaceWord("on", "off", "Turn on the television I want to keep the television on"));
		System.out.println(sp.replaceWord("love", "hate", "I love the World I lovelove the Love"));
	}*/
	public static String removeDuplicatedWords(String sentence){		
		String[] array = sentence.split(" ");
		String ans=array[0];
		for (int i=1; i<array.length; i++){
			if ( ans.indexOf(array[i]) == -1 ){
				ans=ans+" ";
				ans=ans+array[i];
			}
		}
		return ans;
	}
	public static String replaceWord(String target, String replacement, String sentence){
		String[] array = sentence.split(" ");
		String ans="";
		if ( array[0].equals(target)){
			ans=ans+replacement;
		}
		else{
			ans=ans+array[0];
		}
		for (int i=1; i<array.length; i++){
			if ( array[i].equals(target)){
				ans=ans+" ";
				ans=ans+replacement;
			}
			else{
				ans=ans+" ";
				ans=ans+array[i];
			}
		}
		return ans;
	}
}