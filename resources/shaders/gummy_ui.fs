#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

// u_mode: 0=jelly pill/rect, 1=jelly 5-star, 2=mint gingham, 3=polka card, 4=pink photo box
uniform float u_mode;
uniform vec2 u_size;
uniform float u_radius;
uniform float u_pad;
uniform vec4 u_col_core;
uniform vec4 u_col_glow;
uniform float u_glass;
uniform float u_concave;
uniform float u_sparkle;

out vec4 finalColor;

float sd_round_box(vec2 p, vec2 b, float r) {
    vec2 q = abs(p) - b + vec2(r);
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r;
}

float sd_star5(vec2 p, float r, float rf) {
    const vec2 k1 = vec2(0.809016994375, -0.587785252292);
    const vec2 k2 = vec2(-k1.x, k1.y);
    p.x = abs(p.x);
    p -= 2.0 * max(dot(k1, p), 0.0) * k1;
    p -= 2.0 * max(dot(k2, p), 0.0) * k2;
    p.x = abs(p.x);
    p.y -= r;
    vec2 ba = rf * vec2(-k1.y, k1.x) - vec2(0.0, 1.0);
    float h = clamp(dot(p, ba) / dot(ba, ba), 0.0, r);
    return length(p - ba * h) * sign(p.y * ba.x - p.x * ba.y);
}

float hash21(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

vec4 shade_jelly(vec2 p, vec2 half_b, float d, float min_half, float radius) {
    float aa = 1.15;

    // Soft colored candy drop shadow
    vec2 p_sh = p - vec2(0.0, 4.5);
    float d_sh = (u_mode < 0.5)
        ? sd_round_box(p_sh, half_b, radius)
        : (sd_star5(vec2(p_sh.x, -p_sh.y), min_half * 0.88, 0.50) - min_half * 0.14);
    float sh_blur = mix(8.5, 6.5, u_glass);
    float sh_alpha = exp(-max(d_sh + 1.0, 0.0) / sh_blur) * smoothstep(sh_blur * 2.2, -2.0, d_sh);
    sh_alpha *= mix(0.32, 0.19, u_glass);
    vec3 sh_col = mix(u_col_core.rgb * 0.72 + vec3(0.06, 0.04, 0.08), vec3(0.50, 0.64, 0.72), u_glass);

    float fill_mask = 1.0 - smoothstep(-aa, aa, d);
    if (fill_mask <= 0.001) {
        return vec4(sh_col, sh_alpha);
    }

    float depth = clamp(-d / max(min_half, 1.0), 0.0, 1.0);
    vec2 np = p / max(half_b, vec2(1.0));

    vec3 body;
    float body_a;

    if (u_glass > 0.5) {
        // Frosted milky-ice translucent glass dish / track
        vec3 ice_top = vec3(0.93, 0.98, 1.00);
        vec3 ice_mid = vec3(0.82, 0.93, 0.99);
        vec3 ice_deep = vec3(0.70, 0.86, 0.96);

        float grad = clamp(0.5 + 0.42 * np.y + 0.18 * np.x, 0.0, 1.0);
        body = mix(ice_top, ice_mid, grad);
        if (u_concave > 0.5) {
            float bowl = smoothstep(0.0, 0.85, depth);
            body = mix(ice_top, ice_deep, bowl * 0.55 + grad * 0.35);
        } else {
            body = mix(body, ice_deep, smoothstep(0.0, 0.35, 1.0 - depth) * 0.35);
        }

        // Crisp glossy white outer rim
        float rim = 1.0 - smoothstep(0.0, 3.2, -d);
        body = mix(body, vec3(1.0, 1.0, 1.0), rim * 0.88);

        // Top-left curved glass highlight
        float spec_band = smoothstep(0.45, -0.65, np.y) * smoothstep(0.0, 0.25, depth) * (1.0 - smoothstep(0.32, 0.72, depth));
        body = mix(body, vec3(1.0), spec_band * 0.55);

        // Tiny bright glass sheen dot near top-left
        vec2 dot_c = vec2(-half_b.x * 0.42, -half_b.y * 0.42);
        if (half_b.x > half_b.y * 1.4) {
            dot_c = vec2(-half_b.x + min_half * 0.72, -half_b.y * 0.38);
        }
        float glint = 1.0 - smoothstep(min_half * 0.12, min_half * 0.26, length(p - dot_c));
        body = mix(body, vec3(1.0), glint * 0.65);

        body_a = mix(0.88, 0.96, rim);
    } else {
        // Juicy glossy translucent gummy candy
        // Deeper saturated core, bright luminous caustic glow toward bottom-right inner rim
        float light_dir = clamp(0.5 - 0.38 * np.y - 0.22 * np.x, 0.0, 1.0);
        body = mix(u_col_core.rgb, u_col_glow.rgb, 0.22 + 0.34 * light_dir);

        // Subsurface bottom caustic bounce crescent (gives wet translucent gummy feel)
        float caustic = smoothstep(0.04, 0.34, depth) * (1.0 - smoothstep(0.36, 0.85, depth))
                      * smoothstep(-0.25, 0.75, np.y);
        body = mix(body, u_col_glow.rgb * 1.18 + vec3(0.06), caustic * 0.55);

        // Soft glowing translucent outer candy rim (1.5px - 3px)
        float outer_rim = 1.0 - smoothstep(0.0, 3.4, -d);
        body = mix(body, mix(u_col_glow.rgb, vec3(1.0), 0.38), outer_rim * 0.72);

        // Subtle dark depth rim right inside the top-bottom shadow edge for plump 3D volume
        float inner_lip = smoothstep(0.0, 2.5, -d) * (1.0 - smoothstep(2.5, 7.0, -d)) * smoothstep(0.1, 0.9, np.y);
        body *= (1.0 - 0.10 * inner_lip);

        // Signature top-left glossy specular pill reflection
        vec2 hi_half = vec2(max(half_b.x - min_half * 0.42, min_half * 0.22), max(min_half * 0.16, 2.5));
        vec2 hi_pos = vec2(-half_b.x * 0.06, -half_b.y + min_half * 0.36);
        if (half_b.x < half_b.y * 1.25) {
            // Circular/square candy: compact kidney-bean highlight at upper-left
            vec2 q = p - vec2(-min_half * 0.24, -min_half * 0.38);
            float cs = 0.866, sn = -0.5;
            vec2 rq = vec2(q.x * cs - q.y * sn, q.x * sn + q.y * cs);
            float d_hi = sd_round_box(rq, vec2(min_half * 0.25, min_half * 0.11), min_half * 0.10);
            float hi = 1.0 - smoothstep(-1.2, 1.8, d_hi);
            float d_dot = length(p - vec2(min_half * 0.12, -min_half * 0.48)) - min_half * 0.075;
            float hi_dot = 1.0 - smoothstep(-0.8, 1.2, d_dot);
            body = mix(body, vec3(1.0, 0.995, 0.99), clamp(hi * 0.68 + hi_dot * 0.72, 0.0, 0.82));
        } else {
            // Pill / bar candy: sleek top ribbon + bright left bead
            float d_hi = sd_round_box(p - hi_pos, hi_half, hi_half.y);
            float hi = (1.0 - smoothstep(-1.0, 2.4, d_hi)) * smoothstep(0.05, 0.22, depth);
            vec2 bead_c = vec2(-half_b.x + min_half * 0.58, -half_b.y + min_half * 0.36);
            float bead = 1.0 - smoothstep(min_half * 0.10, min_half * 0.22, length(p - bead_c));
            body = mix(body, vec3(1.0, 0.995, 0.99), clamp(hi * 0.44 + bead * 0.58, 0.0, 0.78));
        }

        // Optional subtle sugar sparkles inside blue slider bar
        if (u_sparkle > 0.5) {
            vec2 cell = floor((p + half_b) / 9.0);
            vec2 lc = fract((p + half_b) / 9.0) - 0.5;
            float rnd = hash21(cell);
            if (rnd > 0.84) {
                float sp = 1.0 - smoothstep(0.06, 0.22, length(lc));
                body = mix(body, vec3(0.85, 0.96, 1.0), sp * 0.55);
            }
        }

        body_a = u_col_core.a;
    }

    // Composite body over shadow
    float out_a = fill_mask * body_a + sh_alpha * (1.0 - fill_mask * body_a);
    vec3 out_rgb = (body * (fill_mask * body_a) + sh_col * (sh_alpha * (1.0 - fill_mask * body_a))) / max(out_a, 1e-4);
    return vec4(out_rgb, out_a);
}

void main() {
    vec2 px = fragTexCoord * u_size;
    vec2 center = u_size * 0.5;
    vec2 p = px - center;

    if (u_mode > 1.5 && u_mode < 2.5) {
        // Mode 2: Mint gingham tablecloth (overlapping translucent stripes)
        float tile = max(u_radius, 8.0);
        float band_x = smoothstep(-0.15, 0.15, sin(px.x * 3.14159265 / tile));
        float band_y = smoothstep(-0.15, 0.15, sin(px.y * 3.14159265 / tile));
        vec3 cream = vec3(0.988, 0.980, 0.945);
        vec3 mint = vec3(0.62, 0.88, 0.86);
        vec3 col = cream;
        col = mix(col, mint, band_x * 0.42);
        col = mix(col, mint, band_y * 0.42);
        float weave = (sin(px.x * 1.9) * sin(px.y * 1.9)) * 0.006;
        finalColor = vec4(col + vec3(weave), 1.0);
        return;
    }

    if (u_mode > 2.5 && u_mode < 3.5) {
        // Mode 3: Warm buttercream polka-dot product card
        vec2 half_b = max(u_size * 0.5 - vec2(u_pad), vec2(2.0));
        float r = min(u_radius, min(half_b.x, half_b.y));
        float d = sd_round_box(p, half_b, r);

        float d_sh = sd_round_box(p - vec2(0.0, 5.0), half_b, r);
        float sh_a = exp(-max(d_sh + 1.0, 0.0) / 10.0) * smoothstep(22.0, -2.0, d_sh) * 0.16;
        vec3 sh_col = vec3(0.45, 0.56, 0.56);

        float mask = 1.0 - smoothstep(-1.0, 1.0, d);
        vec3 base = vec3(0.992, 0.961, 0.902);
        vec3 dot_col = vec3(0.945, 0.875, 0.788);

        float spacing = max(u_sparkle, 10.0);
        vec2 cell = mod(px, spacing) - spacing * 0.5;
        float dot_m = 1.0 - smoothstep(spacing * 0.17, spacing * 0.25, length(cell));
        vec3 col = mix(base, dot_col, dot_m * 0.85);

        // Soft cream border rim
        float border = smoothstep(-3.0, -0.5, d);
        col = mix(col, vec3(0.965, 0.910, 0.845), border * 0.75);

        float out_a = mask + sh_a * (1.0 - mask);
        vec3 out_rgb = (col * mask + sh_col * sh_a * (1.0 - mask)) / max(out_a, 1e-4);
        finalColor = vec4(out_rgb, out_a);
        return;
    }

    if (u_mode > 3.5 && u_mode < 4.5) {
        // Mode 4: Blush pink studio photo box with diagonal gloss band + soft floor shadow
        vec2 half_b = max(u_size * 0.5 - vec2(u_pad), vec2(2.0));
        float r = min(u_radius, min(half_b.x, half_b.y));
        float d = sd_round_box(p, half_b, r);
        float mask = 1.0 - smoothstep(-1.0, 1.0, d);

        vec2 uv = clamp((p + half_b) / (half_b * 2.0), 0.0, 1.0);
        vec3 top_c = vec3(0.992, 0.855, 0.863);
        vec3 bot_c = vec3(0.973, 0.788, 0.816);
        vec3 col = mix(top_c, bot_c, uv.y);

        // Diagonal glossy white studio light sweep
        float diag = abs((uv.x - 0.42) * 0.85 + (uv.y - 0.52) * 0.52);
        float sheen = exp(-diag * diag * 18.0) * 0.36;
        col = mix(col, vec3(1.0, 0.975, 0.985), sheen);

        // Soft elliptical floor shadow beneath the floating gummy bears
        vec2 sp = vec2((uv.x - 0.50) / 0.27, (uv.y - 0.865) / 0.036);
        float floor_sh = exp(-dot(sp, sp) * 1.45) * 0.18;
        col = mix(col, vec3(0.72, 0.56, 0.60), floor_sh);

        // Subtle inner glossy highlight along top/bottom edges
        float edge_hi = 1.0 - smoothstep(0.0, 3.0, -d);
        col = mix(col, vec3(1.0, 0.93, 0.94), edge_hi * 0.45 * (1.0 - uv.y));
        float bot_edge = smoothstep(-2.5, 0.0, d) * smoothstep(0.5, 1.0, uv.y);
        col = mix(col, vec3(0.86, 0.66, 0.70), bot_edge * 0.45);

        finalColor = vec4(col, mask);
        return;
    }

    vec2 half_b = max(u_size * 0.5 - vec2(u_pad), vec2(2.0));
    float min_half = min(half_b.x, half_b.y);
    float r = min(u_radius, min_half);
    float d = (u_mode < 0.5)
        ? sd_round_box(p, half_b, r)
        : (sd_star5(vec2(p.x, -p.y), min_half * 0.88, 0.50) - min_half * 0.14);

    finalColor = shade_jelly(p, half_b, d, min_half, r);
}
