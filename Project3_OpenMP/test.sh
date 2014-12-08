for i in `seq 1 10`
do
	echo "------"
	./spell_seq heroes | grep -i "spell check" 
	./spell_t4_singleloop heroes | grep -i "spell check"
	./spell_t4_fastest heroes | grep -i "spell check"
	./spell_t8_singleloop heroes | grep -i "spell check"
	./spell_t8_fastest heroes | grep -i "spell check"
done
