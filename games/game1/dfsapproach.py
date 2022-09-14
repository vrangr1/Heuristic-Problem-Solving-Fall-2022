debugFlag = False

class Solution:
	seen = {}
	# Binary helper methods

	def read_choices(self, choices):
		"""
		Helper method to print out the choices in a human readable way
		"""
		choices = bin(choices)[2:]
		choices = choices.zfill(15)
		choices = choices[::-1]
		return choices

	def smallest_choice(self, num):
		"""
		So num = 0b110000000000000 implies player has card 1 and 2 left, the rest are used/don't exist
		16 - num.bit_length() implies the smallest card left is 16 - 15 = 1
		"""
		return 16 - num.bit_length() # Get index of smallest choice available 

	def check_choice(self, num, choice):
		if choice > 15:
			return False
		return num & (1 << (15-choice)) != 0

	def get_p1_choice(self, num, choice):
		if choice > 15:
			return False
		return num & (1 << (30-choice)) != 0 # From range 30 to 16
	
	def get_p2_choice(self, num, choice):
		if choice > 15:
			return False
		return num & (1 << (15-choice)) != 0 # From range 15 to 1

	def get_new_choices(self, choices, index):
		return choices ^ (1 << (15-index))

	def p1_new_choices(self, num, index):
		return num ^ (1 << (30-choice))

	def p2_new_choices(self, num, index):
		return num ^ (1 << (15-index))

	def canIWin(self, intRange, desiredTotal):
		def can_win(choices, remainder, turn_num):
			
			seen_key = choices
			if self.smallest_choice(choices) > remainder:
				if debugFlag: print("1 ", self.read_choices(choices), remainder, turn_num)
				self.seen[seen_key] = False
				return False
			elif self.check_choice(choices, remainder):
				if debugFlag: print("2 ", self.read_choices(choices), remainder, turn_num)
				self.seen[seen_key] = True
				return True
			elif seen_key in self.seen:
				if debugFlag: print("3 ", self.read_choices(choices), remainder, turn_num)
				# if we have seen this exact scenario play out, then we know the outcome
				return self.seen[seen_key]

			# we haven't won yet.. it's the next player's turn.
			for index in range(15):
				if self.check_choice(choices, index):
					# we can choose this card
					new_choices = self.get_new_choices(choices, index)
					if not can_win(new_choices, remainder - index, turn_num + 1):
						if turn_num == 0: # To get the first number to pick and play
							print(index)
						if debugFlag: print("4 ", self.read_choices(choices), remainder, turn_num)
						self.seen[seen_key] = True
						return True

			# uh-oh if we got here then next player won all permutations, we can't force their hand
			# actually, they were able to force our hand :(
			if debugFlag: print("5 ", self.read_choices(choices), remainder, turn_num)
			self.seen[seen_key] = False
			return False

		# let's do some quick checks before we journey through the tree of permutations
		summed_choices = (intRange + 1) * intRange / 2
		if summed_choices < desiredTotal:
			return False
		elif summed_choices == desiredTotal:
			return True if intRange % 2 == 0 else False

		# Run the tree search
		choices = 2**(intRange)- 1 # The bitmask number, essentially iR=1 -> 1, iR=2 -> 11, iR=3 -> 111, etc.
		choices = choices << 15 - intRange # Shift the bitmask to the left to make it 15 bits long
		answer = can_win(choices, desiredTotal, 0)
		return answer

temp = Solution()
print(temp.canIWin(4, 6))
# Print the dictionary cases
# print(temp.seen)
# print(temp.get_largest_1_bit(7))