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

    XCTAssert(vec2(2, 2).scale(0.5) == vec2(1, 1));

    XCTAssert(vec2(1, 1).add(vec2(2,3)) == vec2(3,4));

    XCTAssert(vec2(4, 3).sub(vec2(2,1)) == vec2(2,2));

    XCTAssert(vec2(4, 3).sqMag() == 25);

    XCTAssert(vec2(4, 3).mag() == 5);

    XCTAssert(vec2(1,1).distance(vec2(4,5)) == 5);

    vec2 v0(1,0);
    vec2 v1(0,1);
    f32 phi = LE_DEG2RAD_F*90.0f;

    f32 f0 = v0.dot(v1);
    f32 f1 = v0.mag() * v1.mag() * cosf(phi);

    XCTAssert(fabs(f0 - f1) < FLT_EPSILON);
}

-(void)testRect {
    rect r(0,0,3,4);
    vec2 p0(0,0);
    vec2 p1(-1, -1);
    vec2 p2(2,2);
    vec2 p3(3,4);
    vec2 p4(3,5);

    XCTAssert(p0.isInside(r));
    XCTAssert(!p1.isInside(r));
    XCTAssert(p2.isInside(r));
    XCTAssert(!p3.isInside(r));
    XCTAssert(!p4.isInside(r));
}


@end
