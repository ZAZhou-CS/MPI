// //Week1 d ay1
// //Pointer
// #include <iostre am>
// using n amesp ace std;
// int m ain(){
// 	int  a = 100;
// 	int &b =  a;
// 	int c = b;
// 	int* p1 = & a;
// 	int* p2 = &b;
// 	int* p3 = &c;
// 	cout<< " a的地址是"<<p1<<"  a的值是 "<<*p1<<endl;
// 	cout<< "b的地址是"<<p2<<" b的值是 "<<*p2<<endl;
// 	cout<< "c的地址是"<<p3<<" c的值是 "<<*p3<<endl;
//         /*
//          a的地址是0x7ffe2f0c a a50 a的值是 100
//         b的地址是0x7ffe2f0c a a50b的值是 100
//         c的地址是0x7ffe2f0c a a54c的值是 100
        
        
//         */
// 	 a=200;
//         cout<< " a的地址是"<<p1<<" a的值是 "<<*p1<<endl;
//         cout<< "b的地址是"<<p2<<"b的值是 "<<*p2<<endl;
//         cout<< "c的地址是"<<p3<<"c的值是 "<<*p3<<endl;
//         /*
//          a的地址是0x7ffe2f0c a a50 a的值是 200
//         b的地址是0x7ffe2f0c a a50b的值是 200
//         c的地址是0x7ffe2f0c a a54c的值是 100
        
//         */
// 	b=200;
//         cout<< " a的地址是"<<p1<<" a的值是 "<<*p1<<endl;
//         cout<< "b的地址是"<<p2<<"b的值是 "<<*p2<<endl;
//         cout<< "c的地址是"<<p3<<"c的值是 "<<*p3<<endl;
//         /*
//          a的地址是0x7ffe2f0c a a50 a的值是 200
//         b的地址是0x7ffe2f0c a a50b的值是 200
//         c的地址是0x7ffe2f0c a a54c的值是 200
        
//         */
// 	c=200;
//         cout<< " a的地址是"<<p1<<" a的值是 "<<*p1<<endl;
//         cout<< "b的地址是"<<p2<<"b的值是 "<<*p2<<endl;
//         cout<< "c的地址是"<<p3<<"c的值是 "<<*p3<<endl;
//         /*
//          a的地址是0x7ffe2f0c a a50 a的值是 500
//         b的地址是0x7ffe2f0c a a50b的值是 500
//         c的地址是0x7ffe2f0c a a54c的值是 200
        
        
//         */

// 	*p1 =500;
// 	cout<< " a的地址是"<<p1<<" a的值是 "<<*p1<<endl;
//         cout<< "b的地址是"<<p2<<"b的值是 "<<*p2<<endl;
//         cout<< "c的地址是"<<p3<<"c的值是 "<<*p3<<endl;
//         /*
//          a的地址是0x7ffe2f0c a a50 a的值是 1000
//         b的地址是0x7ffe2f0c a a50b的值是 1000
//         c的地址是0x7ffe2f0c a a54c的值是 200
        
//         */

//         *p2 =1000;
//         cout<< " a的地址是"<<p1<<" a的值是 "<<*p1<<endl;
//         cout<< "b的地址是"<<p2<<"b的值是 "<<*p2<<endl;
//         cout<< "c的地址是"<<p3<<"c的值是 "<<*p3<<endl;
//         /*
//          a的地址是0x7ffe2f0c a a50 a的值是 1000
//         b的地址是0x7ffe2f0c a a50b的值是 1000
//         c的地址是0x7ffe2f0c a a54c的值是 1000
        
//         */
//         *p3 =1000;
//         cout<< " a的地址是"<<p1<<" a的值是 "<<*p1<<endl;
//         cout<< "b的地址是"<<p2<<"b的值是 "<<*p2<<endl;
//         cout<< "c的地址是"<<p3<<"c的值是 "<<*p3<<endl;

//         /*
        
//          a的地址是0x7ffe2f0c a a50 a的值是 1000
//         b的地址是0x7ffe2f0c a a50b的值是 1000
//         c的地址是0x7ffe2f0c a a54c的值是 1000
//         */

// }



// #include<iostream>
// using namespace std;
// int main()
// {

// 	int a[5]={10,20,30,40,50};
// 	int* p=a;
//         cout << "===== 1. 地址 =====" << endl;
// 	cout<<"  a=  "<<a<<endl;
// 	cout<<" &a[0]=  "<<&a[0]<<endl;
// 	cout<<"	&a[1]=  "<<&a[1]<<endl;
// 	cout<<"	p=  "<<p<<endl;
// 	cout<<endl;
//         /*
//         ===== 1. 地址 =====
//         a=  0x7ffd992ca050   a就是该数组的第一个数字的地址  a=&a[0]
//         &a[0]=  0x7ffd992ca050
// 	&a[1]=  0x7ffd992ca054
// 	p=  0x7ffd992ca050

//         */
// 	cout << "===== 2. 第一个元素的值 =====" << endl;
// 	cout<<" a[0]= "<<a[0]<<endl;
// 	cout<<" *p = "<<*p<<endl;
// 	cout<<endl;
//         /*
//         ===== 2. 第一个元素的值 =====
//         a[0]= 10
//         *p = 10

//         */

// 	cout << "===== 3. pointer + 1 =====" << endl;
// 	cout<<" p =  "<<p<<endl;
// 	cout<<" p+1= "<<p+1<<endl;
// 	cout<<" p+2= "<<p+2<<endl;
// 	cout<<" p+3= "<<p+3<<endl;
// 	cout<<" p+4= "<<p+4<<endl;
// 	cout<<endl;
//         /*
//          p =  0x7ffd992ca050
//         p+1= 0x7ffd992ca054
//         p+2= 0x7ffd992ca058
//         p+3= 0x7ffd992ca05c
//         p+4= 0x7ffd992ca060     
//         */
// 	cout << "===== 4. pointer 访问 array =====" << endl;

//    	 cout << "*p         = " << *p << endl;
//    	 cout << "*(p + 1)   = " << *(p + 1) << endl;
//    	 cout << "*(p + 2)   = " << *(p + 2) << endl;
//     	cout << "*(p + 3)   = " << *(p + 3) << endl;
//    	 cout << "*(p + 4)   = " << *(p + 4) << endl;

//    	 cout << endl;
//         /*
//         ===== 4. pointer 访问 array =====
//         *p         = 10
//         *(p + 1)   = 20
//         *(p + 2)   = 30
//         *(p + 3)   = 40
//         *(p + 4)   = 50
//         */ 

//    	cout << "===== 5. array 下标 vs pointer =====" << endl;

//     	cout << "a[0]       = " << a[0] << endl;
//    	cout << "*(a + 0)   = " << *(a + 0) << endl;

//    	cout << "a[1]       = " << a[1] << endl;
//         cout << "*(a + 1)   = " << *(a + 1) << endl;

//         cout << "a[2]       = " << a[2] << endl;
//         cout << "*(a + 2)   = " << *(a + 2) << endl;

//         cout << endl;
//         /*
//         ===== 5. array 下标 vs pointer =====
//         a[0]       = 10
//         *(a + 0)   = 10
//         a[1]       = 20
//         *(a + 1)   = 20
//         a[2]       = 30
//         *(a + 2)   = 30
     
//         */

//         cout << "===== 6. 修改 array =====" << endl;

//         *p = 100;
//         *(p + 1) = 200;
//         *(p + 2) = 300;

//         cout << "a[0] = " << a[0] << endl;
//         cout << "a[1] = " << a[1] << endl;
//         cout << "a[2] = " << a[2] << endl;

//         return 0;

//         /*
//         ===== 6. 修改 array =====
//         a[0] = 100
//         a[1] = 200
//         a[2] = 300
//         */


// }
// #include<iostream>
// #include<vector>
// using namespace std;
// int main()
// {
// 	vector <int> a={10,20,30,40,50,60};
// 	cout << "size = "<< a.size() << endl;
// 	for(size_t i = 0; i < a.size(); i++) 
// 	{
// 		cout << a[i] <<" ";
// 	} 
// 	cout  << endl;
// 	return 0;
// }


//buffer学习
/*
buffer是一块用来临时存放数据的内存空间
buffer 是“这块内存现在被拿来装/传输/处理数据”的一种角色

*/
// #include <iostream>
// #include <vector>

// int main() {

//     // 1. Array
//     int data[5] = {10, 20, 30, 40, 50};

//     // 2. Pointer
//     int* p = data;

//     // 3. 通过 pointer 访问 array
//     std::cout << "data[0] = " << data[0] << std::endl;  //10
//     std::cout << "*p      = " << *p << std::endl;       //10

//     // 4. Pointer 可以修改 array
//     p[0] = 100;

//     std::cout << "after modification:" << std::endl;  //100
//     std::cout << "data[0] = " << data[0] << std::endl; //100

//     // 5. 把 array 当作 buffer
//     int* buffer = data;

//     std::cout << "buffer:" << std::endl;

//     for (int i = 0; i < 5; i++) {
//         std::cout << buffer[i] << " ";  //100 20 30 40 50
//     }

//     std::cout << std::endl;

//     // 6. Vector
//     std::vector<int> vec = {1, 2, 3, 4, 5};

//     // 7. vector 的连续内存可以通过 data() 获取
//     int* vector_buffer = vec.data();

//     std::cout << "vector buffer:" << std::endl;

//     for (size_t i = 0; i < vec.size(); i++) {
//         std::cout << vector_buffer[i] << " ";
//     }

//     std::cout << std::endl;

//     return 0;
// }