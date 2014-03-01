    if (typeof define === "function" && define.amd) {
        define(tessellate);
    } else if (typeof module === "object" && module.exports) {
        module.exports = tessellate;
    } else {
        this.tessellate = tessellate;
    }
}();
