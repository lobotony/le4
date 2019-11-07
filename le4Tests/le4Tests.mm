#import <XCTest/XCTest.h>
#import "le4.h"

using namespace le4;

@interface le4Tests : XCTestCase

@end

@implementation le4Tests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testVec2 {
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.

    vec2 v(1,2);
    XCTAssert(v.x == 1);
    XCTAssert(v.y == 2);
}

@end
