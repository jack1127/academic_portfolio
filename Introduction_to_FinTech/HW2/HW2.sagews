︠4f9f5649-2b5f-4dcd-85f4-b533653e4f48s︠
import hashlib

a = 0x0
b = 0x7
p = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
n = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141
d = 2126
h = 0x1
Fn = FiniteField(n)
EC = EllipticCurve(GF(p), [a, b])
GX = 0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798
GY = 0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8
G = EC(GX, GY)

def hash(m):
	m = str(m).encode('utf-8')
	return Integer('0x' + hashlib.sha1(m).hexdigest())

def ecdsa_sign(d, m):
	r = 0
	s = 0
	while s == 0:
		k = 1
		while r == 0:
			k = randint(1, n - 1)
			Q = k * G
			(x1, y1) = Q.xy()
			r = Fn(x1)
		kk = Fn(k)
		e = hash(m)
		s = kk ^ (-1) * (e + d * r)
	return [r, s]


def ecdsa_verify(Q, m, r, s):
	e = hash(m)
	w = s ^ (-1) % n
	u1 = (e * w) % n
	u2 = (r * w) % n
	P1 = Integer(u1) * G
	P2 = Integer(u2) * Q
	X = P1 + P2
	(x, y) = X.xy()
	v = Fn(x)
	return v == r

# TEST
d = 2126
Q = d * G
print("4G = ", 4*G)
print("5G = ", 5*G)
print("Q = ", d*G)

print ("EC Public Key       : ", Q.xy() )
print ("EC Private Key      : ", d)

msg = 'check'
msg2 = 'check'

[r, s] = ecdsa_sign(d, msg)
print ("Signed Message      : ", msg)
print ("Signature     : ")
print (" r = ", r)
print (" s = ", s)

result = ecdsa_verify(Q, msg2, r, s)
print ("Verified Message    : ", msg2)
print ("Verification Result : ", result)
︡768bc2cf-9234-4b9f-8513-c90608c91229︡{"stdout":"4G =  (103388573995635080359749164254216598308788835304023601477803095234286494993683 : 37057141145242123013015316630864329550140216928701153669873286428255828810018 : 1)\n"}︡{"stdout":"5G =  (21505829891763648114329055987619236494102133314575206970830385799158076338148 : 98003708678762621233683240503080860129026887322874138805529884920309963580118 : 1)\n"}︡{"stdout":"Q =  (18264629460660027122333430872777685769232827706330953304888903259906426140455 : 28657135427785770538162175163715604772930272098030282898141692500708206780697 : 1)\n"}︡{"stdout":"EC Public Key       :  (18264629460660027122333430872777685769232827706330953304888903259906426140455, 28657135427785770538162175163715604772930272098030282898141692500708206780697)\n"}︡{"stdout":"EC Private Key      :  2126\n"}︡{"stdout":"Signed Message      :  check\n"}︡{"stdout":"Signature     : \n"}︡{"stdout":" r =  30895749090644921940152199779984962640216216142652401071736691087166726698523\n"}︡{"stdout":" s =  55511212841571986029411155871679428784927127890001969939367866198873586399311\n"}︡{"stdout":"Verified Message    :  check\n"}︡{"stdout":"Verification Result :  True\n"}︡{"done":true}









