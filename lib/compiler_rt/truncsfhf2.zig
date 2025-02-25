const common = @import("./common.zig");
const truncf = @import("./truncf.zig").truncf;

pub const panic = common.panic;

comptime {
    if (common.gnu_f16_abi) {
        @export(__truncsfhf2, .{ .name = "__gnu_f2h_ieee", .linkage = common.linkage });
    } else if (common.want_aeabi) {
        @export(__aeabi_f2h, .{ .name = "__aeabi_f2h", .linkage = common.linkage });
    }
    @export(__truncsfhf2, .{ .name = "__truncsfhf2", .linkage = common.linkage });
}

pub fn __truncsfhf2(a: f32) callconv(.C) common.F16T {
    return @bitCast(common.F16T, truncf(f16, f32, a));
}

fn __aeabi_f2h(a: f32) callconv(.AAPCS) u16 {
    return @bitCast(common.F16T, truncf(f16, f32, a));
}
