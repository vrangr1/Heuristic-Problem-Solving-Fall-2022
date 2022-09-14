debugFlag = False

class Solution:
	seen = {}
	# Binary helper methods

	def read_choices(self, choices):
		"""
		Helper method to print out the choices in a human readable way
		"""
		choices = bin(choices)[2:]
		# choices = choices.zfill(15)
		choices = choices[::-1]
		return choices

	def smallest_choice(self, num, turn_num):
		"""
		So num = 0b110000000000000 implies player has card 1 and 2 left, the rest are used/don't exist
		16 - num.bit_length() implies the smallest card left is 16 - 15 = 1
		"""
		if turn_num%2 == 0:
			num = num >> 15
		else:
			num = num & 0b111111111111111
		return 16 - num.bit_length() # Get index of smallest choice available 

	def check_choice(self, num, choice, turn_num):
		if choice > 15:
			return False
		elif turn_num %2 == 0:
			return num & (1 << (30-choice)) != 0 # From range 30 to 16
		else:
			return num & (1 << (15-choice)) != 0 # From range 15 to 1

	def get_new_choices(self, choices, index, turn_num):
		if turn_num %2 == 0:
			return choices ^ (1 << (30-index))
		else:
			return choices ^ (1 << (15-index))


	def canIWin(self, intRange, desiredTotal):
		def can_win(choices, remainder, turn_num):
			
			match_key = choices
			if self.smallest_choice(choices, turn_num) > remainder:
				if debugFlag: print("1 ", self.read_choices(choices), remainder, turn_num)
				self.seen[match_key] = False
				return False
			elif self.check_choice(choices, remainder, turn_num):
				if debugFlag: print("2 ", self.read_choices(choices), remainder, turn_num)
				self.seen[match_key] = True
				return True
			elif match_key in self.seen:
				if debugFlag: print("3 ", self.read_choices(choices), remainder, turn_num)
				# Stored outcome
				return self.seen[match_key]

			for index in range(15):
				if self.check_choice(choices, index, turn_num):
					new_choices = self.get_new_choices(choices, index, turn_num)
					# Check next player choices, given our specific choices
					if not can_win(new_choices, remainder - index, turn_num + 1):
						if turn_num == 0: # To get the first number to pick and play
							print(index)
						if debugFlag: print("4 ", self.read_choices(choices), remainder, turn_num)
						self.seen[match_key] = True
						return True

			if debugFlag: print("5 ", self.read_choices(choices), remainder, turn_num)
			self.seen[match_key] = False
			return False

		summed_choices = (intRange + 1) * intRange / 2
		if 2*summed_choices < desiredTotal:
			return False # TODO: Replace this
		elif 2*summed_choices == desiredTotal:
			return False # TODO: Replace this

		# Run the tree search
		choices = 2**(intRange)- 1 # The bitmask number, essentially iR=1 -> 1, iR=2 -> 11, iR=3 -> 111, etc.
		choices = choices << 15 - intRange # Shift the bitmask to the left to make it 15 bits long
		choices = choices << 15 | choices
		# print(choices)
		answer = can_win(choices, desiredTotal, 0)
		return answer

temp = Solution()
print(temp.canIWin(4, 9))
# Print the dictionary cases
print(temp.seen)
# print(temp.get_largest_1_bit(7))