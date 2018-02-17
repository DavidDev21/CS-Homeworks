/*
  Name: David Zheng
  Lab 14: Recursion
  Date: 5/6/2017
*/

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <climits>
using namespace std;

// Task 1
bool hasEvenOnes(int number)
{
  if(number <= 1)
  {
    return false;
  }
  else if(number%10 == 1)
  {
    return !(hasEvenOnes(number/10));
  }
  else
  {
    return hasEvenOnes(number/10);
  }
}


// Task 2 Takes a half open range of the two list (assuming both list are equal length)
list<int> sumList(list<int>::const_iterator& firstBegin, const list<int>::const_iterator& firstEnd,
	list<int>::const_iterator& secondBegin, const list<int>::const_iterator& secondEnd, list<int> newList = {})
{
	if (firstBegin == firstEnd)
	{
		return newList;
	}
	else
	{
		newList.push_back(*firstBegin + *secondBegin);
		return sumList(++firstBegin, firstEnd, ++secondBegin, secondEnd, newList);
		
	}
}

// Task 3
struct TNode {
	TNode(int data = 0, TNode *left = nullptr, TNode *right = nullptr)
		: data(data), left(left), right(right) {}
	int data;
	TNode *left, *right;
};

// Finds the max value in the tree
// by comparing the max on the left and right subtree to the root
int max(TNode* root) {
	if (root == nullptr)
	{
		return INT_MIN;
	}
	else
	{
		int leftMax = max(root->left);
		int rightMax = max(root->right);

		if (root->data > leftMax && root->data > rightMax)
			return root->data;

		if (leftMax < rightMax)
			return rightMax;

		if(leftMax > rightMax)
			return leftMax;
	}
}

// Task 4
bool palindrome(char* word, int length)
{
	if (length == 1)
	{
		return true;
	}
	else if (word[0] != word[length - 1])
	{
		return false;
	}
	else
	{
		// length - 2 = cutting off two characters each time
		return palindrome(word + 1, length - 2);
	}
}

// Task 5
int vecBinarySearch(const vector<int>& list, int target, int left, int right)
{
	if (left > right)
	{
		return -1;
	}
	else
	{
		int mid = (left + right) / 2;
		if (list[mid] == target)
		{
			return mid;
		}
		else if (target < list[mid])
		{
			return vecBinarySearch(list, target, left, mid-1);
		}
		else
		{
			return vecBinarySearch(list, target, mid+1, right);
		}
	}
}

// Task 6
void functionF(int n) {
	if (n > 1) {
		cout << 'a';
		functionF(n / 2);
		cout << 'b';
		functionF(n / 2);
	}
	cout << 'c';
}

int main()
{
  cout << "Task 1: determine if a non-negative integer has an even number of ones" << endl;
  cout << boolalpha;
  cout << "1111: "<<hasEvenOnes(1111) << endl;
  cout << "123: " << hasEvenOnes(123) << endl;
  cout << "1312: " << hasEvenOnes(1312) << endl;
  cout << "1312411311: " << hasEvenOnes(1312411311) << endl;
  cout << "==============" << endl;
  cout << "Task 2: return a new list that is the sum of the values in the the two lists passed in" << endl;

  list<int> first {1,9,3};
  list<int> g {4,5,6};

  list<int> z = sumList(first.begin(), first.end(), g.begin(), g.end());

  cout << "Sum of List" << endl;
  for(int x : z)
    cout << x << " ";
  cout << endl << "==============" << endl;
  cout << "Task 3: Max- TREE" << endl;

  TNode a(1), b(2), c(4), d(8, &a, &b), e(16, &c), f(32, &d, &e);
  cout << max(&f) << endl;
  cout << "==============" << endl;
  cout << endl  << "Task 4: Palindrome" << endl;
  cout << endl;
  cout << boolalpha;
  char s[] = "racecar";
  cout << "racecar: " << palindrome(s,7)<< endl;

  char x[] = "nozn";
  cout << "nozn: " << palindrome(s, 4) << endl;
  cout << "==============" << endl;
  cout << "Task 5: Binary Search" << endl;

  vector<int> myVector{ 1,2,3,4,5,6,7,8,9,10 };
  cout << vecBinarySearch(myVector, 4, 0, 2) << endl;
  cout << vecBinarySearch(myVector, 10, 0, 9) << endl;

  cout << "==============" << endl;
  cout << "Task 6: Trace Function" << endl;
  functionF(2); // acbcc
  cout << endl; 
  functionF(4); // aacbccbacbccc

  cout << endl;
}
