Problem statement

In mini library management system, the library personnel need to find a book for the customer. 
	1. There will be so many books with the same author and the library personnel need to find using alphabetical order.
	2. The personnel need to check out few books, and at the same time to record any books being returned by the end as well as to keep track the customer’s information.  This process of searching can be time consuming. 

Task

Create more advance functions to help the library personnel based on the following requirements:
•	To add and keep the book information and borrower information in a file.
•	Allow a customer to borrow a maximum of 5 books.
•	Books that could be loaned must be returned within 5 days starting the day the book was loaned.
•	Assume that each customer can make multiple loans as long as the customer did not exceed the number of books borrowed.
•	Present the books loaned by a customer based on the latest borrowing done.
•	To search and get the information of the borrower when she/he wants to return the book. The search for a customer details is through the customer id which is linked to the   	customer individual profile(customer profile contains information such as customer name, books borrowed and etc).
•	To calculate the overdue penalties based on Table 1 and produce the receipt like shown in Figure 1.
•	Include a menu that allows the customer to choose the operation that the customer wants to perform.
•	To calculate return balance based on amount paid by the customer.
•	To provide reporting function like shown in Figure 2.


Table 1: Overdue/late returns/ penalty fine rates
ITEMS	RATES 
(incurred after due date or hour)
General Loan Books	RM 0.20 per day
Red-spot
(text books & recommended readings by Course/Program)	RM 0.20 per hour
Lost ONE (1) book
*applied only for books not being returned to the library for 12 months, starting from the date it has been borrowed	RM50.00



Mini Library Receipt
Return Item

Receipt No: ______________
Customer Id: ______________
Customer Name: __________________                      
Date: 19/03/2018                                  

Books Title   	Status             Day’s Overdue              		
Book A                   	Overdue	                2 days		
Book B	-	                         -		
			
Total Return Book(s):  2
Overdue: 2 days x RM0.20 = RM0.40 
Total Payment: RM0.40			

Figure 1: Example of Printed Receipt for Return Item












Mini Library Receipt
Weekly Summary

Week: 16/3/2018 – 22/3/2018

No        Library Items	Total  Loan  	Total Return	Lost(if any)
 1         Books	    30	      29	     1
			
 2        Red Book Collection

Total Loan Items:  
Total Return Items:
Total Lost Items:	     7

 37
 29
   1	       7
	      - 

Lost (Book): 1= RM50.00

Total Payment: RM50.00
			

Figure 2: Example of Weekly Summary Report

•	You are encouraged to use your own creativity and ideas to add functionalities to make your system as functional and user-friendly a possible. For example, you may incorporate borrowed date and returned date to compute the number of days a customer borrows an item.

•	Hence, your program should focus on the topics below 
i. Array 
ii. Pointer 
iii. Basic data structures 
iv. File processing 

